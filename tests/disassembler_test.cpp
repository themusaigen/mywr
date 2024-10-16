#include <gtest/gtest.h>

#include "mywr/mywr.hpp"

using namespace mywr::disassembler;

// HDE already "tested" by some of projects like MinHook, etc...
// So we create basic test that verifies anything compiles and runs.
TEST(DisassemblerTest, ProducesBasicDisassembling) {
  const mywr::byte_t code[]{
      0xFF,
      0xE0, // jmp eax
      0x5B, // pop ebx
      0x5D, // pop ebp,
      0xC3, // retn
  };

  mywr::byte_t* ptr{const_cast<mywr::byte_t*>(code)};

  size_t offset{0};
  while (true) {
    instruction insn = disassemble(ptr + offset);
    EXPECT_EQ(insn.opcode, code[offset]);
    EXPECT_TRUE(insn.len > 0);

    if (insn.opcode == 0xC3)
      break;

    offset += insn.len;
  }
}
