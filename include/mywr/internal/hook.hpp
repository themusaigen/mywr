#pragma once

#include <shared/core/address.hpp>
#include <shared/utility.hpp>
#include <shared/function_traits.hpp>

#include <internal/allocator.hpp>
#include <internal/module.hpp>
#include <internal/llmo.hpp>
#include <internal/protect.hpp>
#include <internal/invoker.hpp>

#include <internal/hook/context.hpp>
#include <internal/hook/codegen.hpp>
#include <internal/hook/constants.hpp>
#include <internal/hook/relay.hpp>
#include <internal/hook/types.hpp>
#include <internal/hook/errors.hpp>
#include <internal/hook/utility.hpp>

#include <xbyak/xbyak.h>

#include <zstring_view.hpp>

#include <vector>
#include <cstddef>
#include <utility>
#include <optional>
#include <memory>

namespace mywr::hook {
template<typename Fun>
class hook {
private:
  using callback_t = types::callback<hook<Fun>, Fun>;

  address    m_target{};
  callback_t m_callback{};
  size_t     m_size{};
  bool       m_installed{false};

  address m_trampoline{};

  std::optional<std::vector<byte_t>>    m_original_bytes{std::nullopt};
  std::optional<std::vector<byte_t>>    m_usercode_jump{std::nullopt};
  std::shared_ptr<Xbyak::CodeGenerator> m_codecave;

  context m_context{};

  friend inner::relay_t<hook<Fun>, Fun>;

public:
  hook() noexcept = default;
  hook(address target) noexcept
      : m_target(std::move(target))
      , m_size(utility::get_at_least_n_bytes(m_target, constants::kJumpSize)) {}
  hook(address target, callback_t callback) noexcept
      : hook(std::move(target)) {
    redirect(std::move(callback));
  }
  hook(mpt::zstring_view mod, address target) noexcept
      : hook(module::get_module_handle(mod) + target) {}
  hook(mpt::zstring_view mod, address target, callback_t callback) noexcept
      : hook(mod, std::move(target))
      , m_callback(std::move(callback)) {}
  hook(mpt::wzstring_view mod, address target) noexcept
      : hook(module::get_module_handle(mod) + target) {}
  hook(mpt::wzstring_view mod, address target, callback_t callback) noexcept
      : hook(mod, std::move(target))
      , m_callback(std::move(callback)) {}

  hook(const hook&) noexcept                    = delete;
  hook(hook&& rhs) noexcept                     = delete;
  auto operator=(const hook&) noexcept -> hook& = delete;
  auto operator=(hook&& rhs) noexcept -> hook&  = delete;

  ~hook() noexcept {
    remove();
  }

  auto install() noexcept -> std::optional<hook_error> {
    using namespace mywr::protect;
    using namespace mywr::allocator;
    using namespace mywr::llmo;

    constexpr auto kOffsetToUserCode = 0x05;

    if (m_installed) {
      return {hook_error::AlreadyInstalledError};
    }

    if (!m_target) {
      return {hook_error::InvalidAddressError};
    }

    if (!is_executable(m_target)) {
      return {hook_error::NotExecutableError};
    }

    if (m_size < constants::kJumpSize) {
      return {hook_error::NotEnoughSpaceError};
    }

    const scoped_protect protect{
        m_target, protection::ReadWriteExecute, m_size};
    if (protect.invalid()) {
      return {hook_error::ProtectViolationError};
    }

    if (!m_codecave) {
      auto buffer = allocate(Xbyak::DEFAULT_MAX_CODE_SIZE);
      if (!buffer.has_value()) {
        return {hook_error::AllocateCodecaveError};
      }

      inner::codegenerator codegen{buffer.value()};

      codegen.set_current_context(&m_context);
      codegen.create_label_also_add_trampoline(m_target, m_size);
      codegen.save_context();
      codegen.push_hook_object<Fun>(this);
      codegen.call_relay<hook<Fun>, Fun>();

      m_codecave = codegen.ready();
    } else {
      if (copy(m_codecave->getCode(),
               m_usercode_jump->data(),
               m_usercode_jump->size())) {
        return {hook_error::ReinstallHookError};
      }

      m_installed = true;
      return std::nullopt;
    }

    if (!m_original_bytes) {
      m_original_bytes = std::make_optional<std::vector<byte_t>>();
      m_original_bytes->resize(m_size);

      if (copy(m_original_bytes->data(), m_target, m_size, false)) {
        return {hook_error::BackupCreatingError};
      }
    }

    std::printf("target: %6X\n", m_target.value());
    std::printf("codecave: %6X\n",
                reinterpret_cast<uint32_t>(m_codecave->getCode()));

    if (read<byte_t>(m_target, false) == constants::kX86CallOpcode) {
      m_trampoline = mywr::utility::restore_absolute_address(
          *read<uint32_t>(m_target.value() + 1, false),
          m_target,
          constants::kJumpSize);
    } else {
      m_trampoline = reinterpret_cast<address_t>(m_codecave->getCode()) +
                     kOffsetToUserCode;

      if (write<byte_t>(m_target, constants::kX86JmpOpcode, false)) {
        return {hook_error::WriteMemoryError};
      }
    }

    const auto relative = mywr::utility::get_relative_address(
        m_codecave->getCode(), m_target, constants::kJumpSize);
    if (write<uint32_t>(m_target.value() + 1, relative.value(), false)) {
      return {hook_error::WriteMemoryError};
    }

    if (m_size > constants::kJumpSize) {
      fill(m_target.value() + constants::kJumpSize,
           constants::kNopOpcode,
           m_size - constants::kJumpSize,
           false);
    }

    m_installed = true;
    return std::nullopt;
  }

  auto remove() noexcept -> std::optional<hook_error> {
    using namespace mywr::protect;
    using namespace mywr::llmo;

    using mywr::disassembler::disassembler, mywr::disassembler::instruction;

    if (!m_installed) {
      return {hook_error::AlreadyRemovedError};
    }

    if (!m_target) {
      return {hook_error::InvalidAddressError};
    }

    scoped_protect unprotect(m_target, protection::ReadWriteExecute, m_size);
    if (unprotect.invalid()) {
      return {hook_error::ProtectViolationError};
    }

    disassembler disasm{};
    instruction  insn{disasm.disassemble(m_target)};

    auto unload_hook = [this]() -> std::optional<hook_error> {
      if (copy(m_target, m_original_bytes->data(), m_size, false)) {
        return {hook_error::BackupRestoringError};
      }

      if (!allocator::deallocate(m_codecave->getCode(),
                                 m_codecave->getSize())) {
        return {hook_error::DeallocateCodecaveError};
      }

      m_original_bytes.reset();
      m_codecave.reset();
      m_usercode_jump.reset();

      m_installed = false;

      return std::nullopt;
    };

    auto patch_hook = [this]() -> std::optional<hook_error> {
      constexpr auto kUsercodeJumpSize = 5;

      m_usercode_jump = std::make_optional<std::vector<byte_t>>();
      m_usercode_jump->resize(kUsercodeJumpSize);

      if (copy(m_usercode_jump->data(),
               m_codecave->getCode(),
               kUsercodeJumpSize)) {
        return {hook_error::BackupCreatingError};
      }

      if (fill(m_codecave->getCode(),
               constants::kNopOpcode,
               kUsercodeJumpSize)) {
        return {hook_error::UsercodeJumpRemoveError};
      }

      m_installed = false;

      return std::nullopt;
    };

    for (size_t operand = 0U; operand < insn.operand_count(); ++operand) {
      if (insn.is_relative_operand(operand)) {
        auto destination = static_cast<address_t>(insn.abs(m_target, operand));

        if ((destination ==
             reinterpret_cast<address_t>(m_codecave->getCode())) ||
            (destination == m_trampoline.value())) {
          return unload_hook();
        }

        return patch_hook();
      }
    }

    return unload_hook();
  }

  template<typename In>
  void target(In addr) noexcept {
    m_target = mywr::utility::get_function_address(addr);
    m_size   = utility::get_at_least_n_bytes(m_target, constants::kJumpSize);
  }

  template<typename In>
  void target(mpt::zstring_view mod, In addr) noexcept {
    target(module::get_module_handle(mod) +
           mywr::utility::get_function_address(addr));
  }

  template<typename In>
  void target(mpt::wzstring_view mod, In addr) noexcept {
    target(module::get_module_handle(mod) +
           mywr::utility::get_function_address(addr));
  }

  void redirect(callback_t callback) noexcept {
    m_callback = std::move(callback);
  }

  template<typename... Args>
  [[nodiscard]] auto call(Args&&... args) const noexcept {
    return invoker::invoke<Fun>(m_trampoline, std::forward<Args>(args)...);
  }

  [[nodiscard]] auto callback() const noexcept -> auto& {
    return m_callback;
  }

  [[nodiscard]] auto context() const noexcept -> auto& {
    return m_context;
  }

  [[nodiscard]] auto installed() const noexcept {
    return m_installed;
  }
};
} // namespace mywr::hook
