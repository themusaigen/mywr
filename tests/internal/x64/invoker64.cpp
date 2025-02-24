#include <gtest/gtest.h>

#include <internal/invoker.hpp>

using namespace mywr::invoker;

// On 64-bit architectures, both on Linux and Windows, there is a single calling
// convention for all functions, so no specific tests are required.
static auto sum(int lhs, int rhs) -> int {
  return lhs + rhs;
}

class A {
public:
  // NOLINTBEGIN(*-to-static)
  auto sum(int lhs, int rhs) -> int {
    return lhs + rhs;
  }
  // NOLINTEND(*-to-static)
};

using sum_function_t = decltype(&sum);
using sum_method_t   = decltype(&A::sum);

TEST(Invoker, Function) {
  auto lhs{2};
  auto rhs{2};
  auto expected_result{lhs + rhs};

  EXPECT_EQ(invoke<sum_function_t>(&sum, lhs, rhs), expected_result);
}

TEST(Invoker, Method) {
  A    object;
  auto lhs{2};
  auto rhs{2};
  auto expected_result{lhs + rhs};

  EXPECT_EQ(invoke<sum_method_t>(&A::sum, &object, lhs, rhs), expected_result);
}
