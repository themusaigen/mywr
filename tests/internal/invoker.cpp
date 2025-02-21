#include <gtest/gtest.h>

#include <shared/calling_conventions.hpp>

static auto MYWR_CDECL sum_cdecl(int lhs, int rhs) -> int {
  return lhs + rhs;
}

static auto MYWR_STDCALL sum_stdcall(int lhs, int rhs) -> int {
  return lhs + rhs;
}

static auto MYWR_FASTCALL sum_fastcall(int lhs, int rhs) -> int {
  return lhs + rhs;
}

class A {
public:
  // NOLINTBEGIN(*-to-static)
  auto MYWR_THISCALL sum_thiscall(int lhs, int rhs) -> int {
    return lhs + rhs;
  }
  // NOLINTEND(*-to-static)
};

using sum_cdecl_t    = decltype(&sum_cdecl);
using sum_stdcall_t  = decltype(&sum_stdcall);
using sum_fastcall_t = decltype(&sum_fastcall);
using sum_thiscall_t = decltype(&A::sum_thiscall);

#include <internal/invoker.hpp>

using namespace mywr::invoker;

TEST(Invoker, Cdecl) {
  auto lhs{2};
  auto rhs{2};
  auto expected_result{lhs + rhs};

  EXPECT_EQ(invoke<sum_cdecl_t>(&sum_cdecl, lhs, rhs), expected_result);
}

TEST(Invoker, Stdcall) {
  auto lhs{2};
  auto rhs{2};
  auto expected_result{lhs + rhs};

  EXPECT_EQ(invoke<sum_stdcall_t>(&sum_stdcall, lhs, rhs), expected_result);
}

TEST(Invoker, Thiscall) {
  A    object;
  auto lhs{2};
  auto rhs{2};
  auto expected_result{lhs + rhs};

  EXPECT_EQ(invoke<sum_thiscall_t>(&A::sum_thiscall, &object, lhs, rhs),
            expected_result);
}

TEST(Invoker, Fastcall) {
  auto lhs{2};
  auto rhs{2};
  auto expected_result{lhs + rhs};

  EXPECT_EQ(invoke<sum_fastcall_t>(&sum_fastcall, lhs, rhs), expected_result);
}
