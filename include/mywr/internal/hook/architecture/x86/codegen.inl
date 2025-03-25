#pragma once

#include <shared/core/address.hpp>
#include <shared/disassembler.hpp>
#include <shared/function_traits.hpp>
#include <shared/utility.hpp>

#include <internal/hook/constants.hpp>
#include <internal/hook/context.hpp>
#include <internal/hook/relay.hpp>

#include <type_traits>
#include <xbyak/xbyak.h>

#include <memory>

namespace mywr::hook::inner {
#pragma pack(push, 1)
struct x86_jmp {
  byte_t    opcode{constants::kX86JmpOpcode};
  address_t operand{};
};

struct x86_call {
  byte_t    opcode{constants::kX86CallOpcode};
  address_t operand{};
};
#pragma pack(pop)

class codegenerator {
private:
  std::shared_ptr<Xbyak::CodeGenerator> m_code;

  context* m_context{nullptr};

public:
  codegenerator(const address& buffer)
      : m_code(
            std::make_shared<Xbyak::CodeGenerator>(Xbyak::DEFAULT_MAX_CODE_SIZE,
                                                   buffer)) {}

  void set_current_context(context* context) noexcept {
    m_context = context;
  }

  void create_label_also_add_trampoline(const address& target,
                                        size_t         minimal_bytes) noexcept {
    Xbyak::Label label;

    m_code->jmp(label, Xbyak::CodeGenerator::T_NEAR);

    generate_trampoline(target, minimal_bytes);

    m_code->L(label);
  }

  void save_context() noexcept {
    using namespace Xbyak::util;

    m_code->mov(ptr[&m_context->eax], eax);
    m_code->mov(ptr[&m_context->ebx], ebx);
    m_code->mov(ptr[&m_context->ecx], ecx);
    m_code->mov(ptr[&m_context->edx], edx);
    m_code->mov(ptr[&m_context->esi], esi);
    m_code->mov(ptr[&m_context->edi], edi);
    m_code->mov(ptr[&m_context->ebp], ebp);
    m_code->mov(ptr[&m_context->esp], esp);

    m_code->pop(eax);
    m_code->mov(ptr[&m_context->return_address], eax);
  }

  template<typename Fun>
  void push_hook_object(const address& hook) noexcept {
    using namespace Xbyak::util;

    using Ret = detail::return_type_t<Fun>;

    constexpr auto is_thiscall =
        detail::convention_v<Fun> == detail::calling_conventions::Thiscall;
    constexpr auto is_fastcall =
        detail::convention_v<Fun> == detail::calling_conventions::Fastcall;

    if constexpr (std::is_void_v<Ret>) {
      if constexpr (is_thiscall) {
        m_code->push(ecx);
      }

      m_code->push(hook);
    } else {
      constexpr auto have_hidden_parameter = sizeof(Ret) > 8 ||
                                             !std::is_trivial_v<Ret> ||
                                             !std::is_standard_layout_v<Ret>;

      if constexpr (have_hidden_parameter) {
        m_code->pop(edx);
      }

      if constexpr (is_thiscall) {
        m_code->push(ecx);
      }

      m_code->push(hook);

      if constexpr (have_hidden_parameter) {
        m_code->push(edx);
      }
    }
  }

  template<typename Hook, typename Fun>
  void call_relay() noexcept {
    using namespace Xbyak::util;

    auto* relay = reinterpret_cast<void*>(relay_t<Hook, Fun>::call);
    if constexpr (detail::convention_v<Fun> ==
                  detail::calling_conventions::Cdecl) {
      m_code->call(relay);
      m_code->add(esp, sizeof(uint32_t));
      m_code->jmp(ptr[&m_context->return_address]);
    } else {
      m_code->push(eax);
      m_code->jmp(relay);
    }
  }

  void generate_trampoline(address address, size_t minimal_bytes) noexcept {
    using mywr::disassembler::disassembler;

    auto start_address{address};

    x86_jmp  jmp{};
    x86_call call{};

    disassembler disasm{};
    size_t       size{};
    while (size < minimal_bytes) {
      auto  instruction{disasm.disassemble(address)};
      auto* instruction_ptr{address.pointer<void>()};
      auto  instruction_size{instruction.length()};

      if (instruction.opcode() == constants::kX86CallOpcode) {
        auto destination = static_cast<uint32_t>(instruction.abs(address));

        call.operand = mywr::utility::get_relative_address(
            destination, m_code->getCurr(), instruction_size);

        instruction_ptr  = &call;
        instruction_size = sizeof(call);
      } else if ((instruction.opcode() & constants::kX86JmpMask) ==
                 constants::kX86JmpOpcode) {
        auto destination = static_cast<uint32_t>(instruction.abs(address));

        jmp.operand = mywr::utility::get_relative_address(
            destination, m_code->getCurr(), constants::kJumpSize);

        instruction_ptr  = &jmp;
        instruction_size = sizeof(jmp);
      }

      m_code->db(static_cast<byte_t*>(instruction_ptr), instruction_size);

      address += instruction.length();
      size += instruction_size;
    }

    // NOLINTBEGIN(*-no-int-to-ptr)
    m_code->jmp(reinterpret_cast<void*>(start_address.value() + minimal_bytes));
    // NOLINTEND(*-no-int-to-ptr)
  }

  [[nodiscard]] auto ready() noexcept {
    m_code->ready();

    return m_code;
  }
};
} // namespace mywr::hook::inner
