/*********************************************************************
 * @file
 * disassembler.hpp
 * @brief  Module containing HDE`s wrapper.
 *
 * @author
 * themusaigen
 * @date   October 2024
 * @copyright MIT License.

 * *********************************************************************/
#ifndef MYWR_DISASSEMBLER_HPP_
#define MYWR_DISASSEMBLER_HPP_

#if defined(MYWR_X86)
  #include "hde32.h"
#else
  #include "hde64.h"
#endif

namespace mywr {
/**
 * @brief Namespace containing wrapper about `Hacker Disassembler Engine`.
 */
namespace disassembler {
/**
 * @brief Disassembled instruction structure.
 */
#if defined(MYWR_X86)
using instruction = hde32s;
#else
using instruction = hde64s;
#endif

/**
 * @brief Produces disassembling of specified code.
 *
 * @details
 * Produces architecture specific (x86 or x64) disassembling and returns the
 * structure with results (length, opcode, imm, disp, etc...).
 * 
 * @param[in] code The code to disassemble.
 */
MYWR_INLINE instruction disassemble(const address& code) {
  instruction insn;
#if defined(MYWR_X86)
  hde32_disasm(code, &insn);
#else
  hde64_disasm(code, &insn);
#endif
  return insn;
}
} // namespace disassembler
} // namespace mywr

#endif // !MYWR_DISASSEMBLER_HPP_
