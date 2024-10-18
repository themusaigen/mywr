/*********************************************************************
 * @file   hook32.hpp
 * @brief  Module containing hook32` code-generator.
 * 
 * @author themusaigen
 * @date   October 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_HOOK32_HPP_
#define MYWR_HOOK32_HPP_

/**
 * Dependencies.
 */
#include "context.hpp"
#include "relay.hpp"

/**
 * Implementation.
 */
namespace mywr {
namespace hook {
namespace impl {
class code_generator {
#pragma pack(push, 1)
  struct jmp {
    byte_t    opcode;
    address_t operand;
  };

  struct call : public jmp {};

  struct jcc {
    byte_t    opcode;
    byte_t    condition;
    address_t operand;
  };
#pragma pack(pop)

public:
  code_generator()
      : m_code(std::make_shared<Xbyak::CodeGenerator>()) {}

  /**
   * @brief Appends trampoline and label to jump code.
   */
  void append_trampoline_and_usercode_label(const address& dest,
                                            std::size_t    size) {
    Xbyak::Label L;

    m_code->jmp(L, Xbyak::CodeGenerator::LabelType::T_NEAR);
    m_code->nop(3);

    generate_trampoline(dest, size, m_code.get());

    m_code->L(L);
  }

  /**
   * @brief Sets new context.
   */
  void set_context(context* context) {
    m_context = context;
  }

  /**
   * @brief Copy the most important thing: EIP
   */
  void copy_eip() {
    using namespace Xbyak::util;

    /**
     * Save EAX and EIP.
     */
    m_code->mov(ptr[&m_context->eax], eax);
    m_code->pop(eax);
    m_code->mov(ptr[&m_context->eip], eax);
  }

  void copy_registers() {
    using namespace Xbyak::util;

    m_code->mov(ptr[&m_context->ebx], ebx);
    m_code->mov(ptr[&m_context->ecx], ecx);
    m_code->mov(ptr[&m_context->edx], edx);
    m_code->mov(ptr[&m_context->esp], esp);
    m_code->mov(ptr[&m_context->ebp], ebp);
    m_code->mov(ptr[&m_context->esi], esi);
    m_code->mov(ptr[&m_context->edi], edi);
  }

  /**
   * @brief Push hook object as parameter.
   */
  template<typename Fun>
  void push_hook_object(const address& hook) {
    using namespace Xbyak::util;

    constexpr bool is_hidden_parameter_needed =
        traits::is_return_non_pod_v<Fun>;
    constexpr bool is_thiscall = traits::is_thiscall_v<Fun>;

    if constexpr (is_hidden_parameter_needed)
      m_code->pop(eax);

    if constexpr (is_thiscall)
      m_code->push(ecx);

    m_code->push(hook);

    if constexpr (is_hidden_parameter_needed) {
      m_code->push(eax);
      m_code->mov(eax, ptr[&m_context->eip]);
    }
  }

  /**
   * @brief Call relay based on function signature.
   */
  template<typename Hook, typename Fun>
  void call_relay() {
    using namespace Xbyak::util;

    void* relay_ptr = reinterpret_cast<void*>(relay_t<Hook, Fun>::relay);
    if constexpr (traits::is_cdecl_v<Fun>) {
      m_code->call(relay_ptr);
      m_code->add(esp, 4);
      m_code->jmp(ptr[&m_context->eip]);
    } else {
      m_code->push(eax);
      m_code->jmp(relay_ptr);
    }
  }

  /**
   * @brief Returns code ready to execute.
   */
  auto get() {
    m_code->ready();
    return m_code;
  }

  /**
   * @brief Generates trampoline.
   */
  bool generate_trampoline(address_t             dest,
                           size_t                size,
                           Xbyak::CodeGenerator* code) {
    /**
     * Relative instruction structures.
     */
    jmp  jmp{0xE9, 0x00000000u};
    call call{0xE8, 0x00000000u};
    jcc  jcc{0x0F, 0x80, 0x00000000u};

    /**
     * Loop variables.
     */
    void*  opcode;
    size_t oplen{};
    size_t offset{};

    while (true) {
      using namespace disassembler;

      instruction insn = disassemble(dest);

      if (insn.flags & F_ERROR)
        break;

      /**
       * Setup opcode and oplen by default.
       */
      opcode = reinterpret_cast<void*>(dest);
      oplen  = insn.len;

      // TODO: Fix magic values
      if (insn.opcode == 0xE8) {
        address rip =
            detail::restore_absolute_address(insn.imm.imm32, dest, insn.len);

        call.operand = detail::compute_relative_address(
            rip, code->getCurr(), sizeof(call));

        opcode = &call;
        oplen  = sizeof(call);
      } else if ((insn.opcode & 0xFD) == 0xE9) {
        address_t rip = dest + insn.len;

        if (insn.opcode == 0xEB)
          rip += insn.imm.imm8;
        else
          rip += insn.imm.imm32;

        jmp.operand =
            detail::compute_relative_address(rip, code->getCurr(), sizeof(jmp));

        opcode = &jmp;
        oplen  = sizeof(jmp);
      } else if ((insn.opcode & 0xF0) == 0x70 || (insn.opcode & 0xFC) == 0xE0 ||
                 (insn.opcode2 & 0xF0) == 0x80) {
        address_t rip = dest + insn.len;

        if ((insn.opcode & 0xF0) == 0x70 || (insn.opcode & 0xFC) == 0xE0)
          rip += insn.imm.imm8;
        else
          rip += insn.imm.imm32;

        if ((insn.opcode & 0xFC) == 0xE0) {
          return false;
        } else {
          byte_t condition =
              ((insn.opcode != 0x0F ? insn.opcode : insn.opcode2) & 0x0F);

          jcc.condition = 0x80 | condition;
          jcc.operand   = detail::compute_relative_address(
              rip, code->getCurr(), sizeof(jcc));

          opcode = &jcc;
          oplen  = sizeof(jcc);
        }
      }

      code->db(reinterpret_cast<byte_t*>(opcode), oplen);

      dest += oplen;
      offset += oplen;

      if (offset >= size) {
        code->jmp(reinterpret_cast<void*>(dest));
        break;
      }
    }

    return (offset >= size);
  }

private:
  context* m_context;

  std::shared_ptr<Xbyak::CodeGenerator> m_code;
};
} // namespace impl
} // namespace hook
} // namespace mywr

#endif // !MYWR_HOOK32_HPP_
