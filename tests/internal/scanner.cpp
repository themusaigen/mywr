#include <gtest/gtest.h>

#include <internal/module.hpp>
#include <internal/scanner.hpp>

#include <string>
#include <iostream>

using namespace mywr::scanner;
using namespace mywr::module;

namespace {
template<class T>
auto operator<<(std::ostream& stream, const std::vector<T>& vec)
    -> std::ostream& {
  stream << "[ ";
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    stream << std::to_string(*it) << " ";
  }
  stream << "]";
  return stream;
}
} // namespace

namespace {
void declare_signature_test(const signature_base&            signature,
                            const std::vector<mywr::byte_t>& expected_pattern,
                            const std::vector<bool>& expected_mask) noexcept {
  std::cout << signature.pattern() << " : " << signature.mask() << '\n';
  std::cout << expected_pattern << " : " << expected_mask << '\n';

  // Check if the signature matches the expected pattern and mask.
  EXPECT_TRUE(std::equal(signature.pattern().begin(),
                         signature.pattern().end(),
                         expected_pattern.begin(),
                         expected_pattern.end()));
  EXPECT_TRUE(std::equal(signature.mask().begin(),
                         signature.mask().end(),
                         expected_mask.begin(),
                         expected_mask.end()));
}
} // namespace

// NOLINTBEGIN(*-magic-numbers)
TEST(Signatures, Basic) {
  declare_signature_test(
      basic_signature("\xE8\x00\x00\x00\x00\x45\x33\xF6\x66\x44\x89\x34\x33",
                      "x????xxxxxxxx"),
      {0xE8,
       0x00,
       0x00,
       0x00,
       0x00,
       0x45,
       0x33,
       0xF6,
       0x66,
       0x44,
       0x89,
       0x34,
       0x33},
      {true,
       false,
       false,
       false,
       false,
       true,
       true,
       true,
       true,
       true,
       true,
       true,
       true});
}

TEST(Signatures, Ida) {
  declare_signature_test(ida_signature("E8 ? ? ? ? 45 33 F6 66 44 89 34 33"),
                         {0xE8,
                          0x00,
                          0x00,
                          0x00,
                          0x00,
                          0x45,
                          0x33,
                          0xF6,
                          0x66,
                          0x44,
                          0x89,
                          0x34,
                          0x33},
                         {true,
                          false,
                          false,
                          false,
                          false,
                          true,
                          true,
                          true,
                          true,
                          true,
                          true,
                          true,
                          true});
}

TEST(Signatures, X64Dbg) {
  // Ida signature must support x64dbg signatures too.
  declare_signature_test(
      ida_signature("E8 ?? ?? ?? ?? 45 33 F6 66 44 89 34 33"),
      {0xE8,
       0x00,
       0x00,
       0x00,
       0x00,
       0x45,
       0x33,
       0xF6,
       0x66,
       0x44,
       0x89,
       0x34,
       0x33},
      {true,
       false,
       false,
       false,
       false,
       true,
       true,
       true,
       true,
       true,
       true,
       true,
       true});
}
// NOLINTEND(*-magic-numbers)

TEST(Scanner, Scan) {
  scanner scanner{get_executable_module_handle()};

  EXPECT_TRUE(scanner.scan(ida_signature("E8 ? ? ? ?")));
}
