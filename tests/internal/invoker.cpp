#include <gtest/gtest.h>

#include <shared/calling_conventions.hpp>
#include <internal/invoker.hpp>

using namespace mywr::invoker;

namespace {
auto MYWR_CDECL sum(int lhs, int rhs) -> int {
  return lhs + rhs;
}

auto MYWR_STDCALL sum_std(int lhs, int rhs) -> int {
  return lhs + rhs;
}
} // namespace

TEST(Invoker, CallCdeclFunction) {
  EXPECT_EQ(invoke<decltype(&sum)>(&sum, 2, 2), 4);
}

TEST(Invoker, CallStdcallFunction) {
  EXPECT_EQ(invoke<decltype(&sum_std)>(&sum_std, 2, 2), 4);
}

// Thiscall can be invoked, but we can't deduce signature from non-static
// method. (TODO: FIX)
