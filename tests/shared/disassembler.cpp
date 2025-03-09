#include <gtest/gtest.h>

#include <shared/disassembler.hpp>

using namespace mywr::disassembler;

TEST(Disassembler, Disasm) {
  constexpr auto expected_opcode = mywr::byte_t{0xE8};
  constexpr auto expected_imm    = mywr::address_t{0x00000000};
  constexpr auto expected_length = 5U;
  constexpr auto expected_abs    = mywr::address_t{expected_length};

  const char*        code{"\xE8\x00\x00\x00\x00"};
  const disassembler object{};

  auto result = object.disassemble(code);

  EXPECT_TRUE(result.is_immediate_operand());

  EXPECT_EQ(result.opcode(), expected_opcode);
  EXPECT_EQ(result.imm32(), expected_imm);
  EXPECT_EQ(result.length(), expected_length);
  EXPECT_EQ(result.abs(0x0), expected_abs);
}
