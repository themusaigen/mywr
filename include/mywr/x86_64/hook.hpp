/*********************************************************************
 * @file   hook.hpp
 * @brief  Module containing Hook API.
 * 
 * @author themusaigen
 * @date   October 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_HOOK_HPP_
#define MYWR_HOOK_HPP_

/**
 * Xbyak code-gen.
 */
#include "xbyak/xbyak.h"

/**
 * Hook backends.
 */
#if defined(MYWR_X86)
  #include "..\x86\hook\hook32.hpp"
#else
  #include "..\x64\hook\hook64.hpp"
#endif

namespace mywr {
namespace hook {
namespace impl {
/**
 * @brief Checks is specified code is executable.
 */
MYWR_FORCEINLINE bool is_executable(const address& dest) {
  return (protect::get_protect(dest) & protect::memory_prot::kExecute) > 0;
}

/**
 * @brief Returns the minimum size of instruction on passed dest or bigger if needed.
 */
MYWR_FORCEINLINE std::size_t get_at_least_n_bytes(address_t   dest,
                                                  std::size_t n = 5u) {
  std::size_t size{};

  while (size < n) {
    using namespace disassembler;

    instruction insn = disassemble(dest + size);

    if (insn.flags & F_ERROR)
      break;

    size += insn.len;
  }
  return size;
}
} // namespace impl
} // namespace hook
} // namespace mywr

namespace mywr {
/**
 * @brief Namespace containing hook API.
 */
namespace hook {
namespace impl {
/**
 * @brief Meta-struct to convert reference-type to pointer-type.
 */
template<typename T, typename = void>
struct convert_reference {
  using type = T;
};

template<typename T>
struct convert_reference<T, std::enable_if_t<std::is_reference_v<T>>> {
  using type = std::add_pointer_t<std::remove_cv_t<std::remove_reference_t<T>>>;
};

template<typename T>
using convert_reference_t = typename convert_reference<T>::type;

/**
 * @brief Hook options.
 */
class options {
public:
  enum Enum : std::uint32_t {
    kCollectRegisters
  };
};

/**
 * @brief Hook class providing constant interface for x86 and x64 architecture.
 * 
 * @details
 * Uses two template parameters as trick to implement `call` method with IntelliSense
 * support that invokes original method.
 */
template<typename, typename>
class hook;

template<typename Fun, typename... Args>
class hook<Fun, std::tuple<Args...>> {
private:
  using Ret = traits::return_type_t<Fun>;
  using callback_t =
      std::function<Ret(const hook&, convert_reference_t<Args>...)>;

public:
  hook()            = default;
  hook(const hook&) = delete;
  hook(hook&&)      = delete; /* Temporary removed */

  hook& operator=(const hook&) = delete;
  hook& operator=(hook&&)      = delete;

  hook(const address& dest) {
    set(dest);
  }

  hook(const address& dest, callback_t callback) {
    set(dest);
    set(std::move(callback));
  }

  ~hook() {
    remove();
  }

  bool install() {
    if (m_installed || !m_can_install)
      return false;

    /**
     * Reserve original bytes.
     */
    if (!m_original_bytes) {
      m_original_bytes = std::make_unique<byte_t[]>(m_size);

      /**
       * Copy them.
       */
      llmo::copy(m_original_bytes.get(), m_dest, m_size);
    }

    /**
     * Do code-gen.
     */
    if (!m_code) {
      code_generator code_gen;

      /**
       * Switch to new context.
       */
      code_gen.set_context(&m_context);

      /**
       * Append usercode label.
       */
      code_gen.append_trampoline_and_usercode_label(m_dest, m_size);

      /**
       * Save eip.
       */
      code_gen.copy_eip();

      /**
       * Save GP registers.
       */
      if (m_options[options::kCollectRegisters])
        code_gen.copy_registers();

      /**
       * Push hook object.
       */
      code_gen.push_hook_object<Fun>(this);

      /**
       * Call relay.
       */
      code_gen.call_relay<hook, Fun>();

      /**
       * Get code.
       */
      m_code = code_gen.get();

      std::printf("%X\n", reinterpret_cast<std::uint32_t>(m_code->getCode()));
      std::printf("%X\n", m_dest);

      /**
       * Patch this region to install our hook.
       */
      if (llmo::read<byte_t>(m_dest) != 0xE8) {
        llmo::write<byte_t>(m_dest, 0xE9);

        /**
         * In any way, trampoline will be generated at position `code + 8 bytes`.
         */
        m_trampoline = reinterpret_cast<address_t>(m_code->getCode()) +
                       sizeof(std::uint64_t);
      } else {
        using namespace disassembler;

        /**
         * This is call instruction, so we need to restore absolute address and store it as trampoline.
         */
        instruction insn = disassemble(m_dest);

        m_trampoline =
            detail::restore_absolute_address(insn.imm.imm32, m_dest, insn.len);
      }

      llmo::write<std::uint32_t>(
          m_dest + 1,
          detail::compute_relative_address(m_code->getCode(), m_dest));

      if (m_size > 5)
        llmo::fill(m_dest + 5, 0x90, m_size - 5);
    } else {
      /**
       * Jumpcode already generated, so we need just to restore usercode jump.
       */
      m_code->rewrite(0, m_usercode_backup, sizeof(std::uint64_t));
    }

    m_installed = true;
    return true;
  }

  bool remove() {
    using namespace disassembler;

    if (!m_installed)
      return false;

    /**
     * Patch hook utility.
     */
    auto patch_hook = [this]() {
      /**
       * Make backup of `jmp usercode`.
       */
      std::memcpy(&m_usercode_backup, m_code->getCode(), sizeof(std::uint64_t));

      /**
       * Nop this jump.
       */
      m_code->rewrite(0, 0x90'90'90'90'90'90'90'90, 8);

      /**
       * Change flags.
       */
      m_installed = false;
      return true;
    };

    /**
     * Fully unload hook utility.
     */
    auto unload_hook = [this](bool executable = true) {
      /**
       * Restore original bytes.
       */
      if (executable)
        llmo::copy(m_dest, m_original_bytes.get(), m_size);

      /**
       * Reset all buffers.
       */
      m_original_bytes.reset();
      m_code.reset();

      /**
       * Change flags.
       */
      m_installed = false;
      return true;
    };

    if (!is_executable(m_dest))
      return unload_hook(false);

    instruction insn = disassemble(m_dest);

    /**
     * Listing got broken, anything else.
     */
    if (insn.flags & F_ERROR)
      return unload_hook();

    /**
     * No relative operations in these area.
     */
    if (!(insn.flags & F_RELATIVE))
      return unload_hook();

    /**
     * This is not rel32 operation.
     */
    if (!(insn.flags & F_IMM32))
      return unload_hook();

    /**
     * We got relative instruction at this point, we need restore absolute address
     * and check if it pointing at our trampoline, then fully unload hook
     * otherwise patch hook.
     */
    address_t abs = detail::restore_absolute_address(insn.imm.imm32, m_dest);

    if (abs == m_trampoline)
      return unload_hook();
    else
      return patch_hook();
  }

  /**
   * @brief Set new hook destination.
   */
  void set(const address& dest) {
    m_dest        = dest.value();
    m_size        = get_at_least_n_bytes(m_dest);
    m_can_install = is_executable(m_dest) && m_size >= 5u;
  }

  /**
   * @brief Sets new callback.
   */
  void set(callback_t callback) {
    m_callback = std::move(callback);
  }

  /**
   * @brief Enables or disables hook` feature.
   */
  void toggle_option(options::Enum option, bool state) {
    m_options[option] = state;
  }

  /**
   * @brief Returns is hook installed.
   */
  bool installed() const {
    return m_installed;
  }

  /**
   * @brief Returns hook callback to use in relay.
   */
  auto& callback() const {
    return m_callback;
  }

  /**
   * @brief Returns current return address.
   */
  auto& eip() const {
    return m_context.eip;
  }

  /**
   * @brief Returns current CPU context.
   */
  const auto& context() const {
    return m_context;
  }

  /**
   * @brief Call original function.
   */
  Ret call(Args... args) const {
    using namespace invoker;
    return invoke<Fun>(m_trampoline, std::forward<Args>(args)...);
  }

private:
  address_t  m_dest{};
  callback_t m_callback;

  size_t m_size{};

  std::unique_ptr<byte_t[]> m_original_bytes;

  std::shared_ptr<Xbyak::CodeGenerator> m_code;

  std::bitset<8> m_options{};

  impl::context m_context{};

  std::uint64_t m_usercode_backup{};

  address_t m_trampoline{};

  bool m_installed{};
  bool m_can_install{};
};

} // namespace impl
} // namespace hook
} // namespace mywr

namespace mywr {
namespace hook {
using options = impl::options;

template<typename Fun>
using hook = impl::hook<Fun, traits::arguments_t<Fun>>;
} // namespace hook
} // namespace mywr

#endif // !MYWR_HOOK_HPP_
