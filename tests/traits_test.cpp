#include <gtest/gtest.h>

#include "mywr/mywr.hpp"

namespace detail = mywr::detail;
namespace traits = mywr::traits;

TEST(TraitsTest, HandlesAbi) {
  ASSERT_FALSE(detail::abi_v == detail::system_abi::kUnknown);

#if defined(MYWR_X86)
  #if defined(MYWR_WINDOWS)
  EXPECT_EQ(detail::abi_v, detail::system_abi::kWin32);
  #elif defined(MYWR_UNIX)
  EXPECT_EQ(detail::abi_v, detail::system_abi::kUnix);
  #endif
#elif defined(MYWR_X64)
  #if defined(MYWR_WINDOWS)
  EXPECT_EQ(detail::abi_v, detail::system_abi::kWin64);
  #elif defined(MYWR_UNIX)
  EXPECT_EQ(detail::abi_v, detail::system_abi::kSystemV);
  #endif
#endif
}

TEST(TraitsTest, HandlesCallingConventions) {
  constexpr auto cc_cdecl =
      detail::calling_convention_by_abi_v<detail::calling_conventions::kCdecl>;
  constexpr auto cc_std = detail::calling_convention_by_abi_v<
      detail::calling_conventions::kStdcall>;
  constexpr auto cc_this = detail::calling_convention_by_abi_v<
      detail::calling_conventions::kThiscall>;
  constexpr auto cc_fast = detail::calling_convention_by_abi_v<
      detail::calling_conventions::kFastcall>;

  ASSERT_FALSE(detail::abi_v == detail::system_abi::kUnknown);

  // This will also test the x64.
  EXPECT_EQ(traits::convention_v<int()>, cc_cdecl);
  EXPECT_EQ(traits::convention_v<int(MYWR_CDECL*)()>, cc_cdecl);
  EXPECT_EQ(traits::convention_v<int(MYWR_STDCALL*)()>, cc_std);
  EXPECT_EQ(traits::convention_v<int MYWR_STDCALL()>, cc_std);
  EXPECT_EQ(traits::convention_v<int(MYWR_THISCALL*)()>, cc_this);
  EXPECT_EQ(traits::convention_v<int(MYWR_FASTCALL*)()>, cc_fast);
}

TEST(TraitsTest, HandlesNonPod) {
  EXPECT_FALSE(traits::is_return_non_pod_v<int()>);
  EXPECT_FALSE(traits::is_return_non_pod_v<double()>);
  EXPECT_TRUE(traits::is_return_non_pod_v<std::string()>);

#if defined(MYWR_UNIX)
  class A {
    int a;
  };

  union B {
    int   a;
    short c;
  };

  EXPECT_TRUE(traits::is_return_non_pod_v<A()>);
  EXPECT_TRUE(traits::is_return_non_pod_v<B()>);
#endif
}

TEST(TraitsTest, HandlesArgsSize) {
  constexpr auto size = sizeof(mywr::byte_t) * 2 + sizeof(int) * 2;

  EXPECT_EQ(traits::args_size_v<void(mywr::byte_t)>, sizeof(mywr::byte_t));
  EXPECT_EQ(traits::args_size_v<void(mywr::byte_t, int, mywr::byte_t, int)>,
            size);
}

TEST(TraitsTest, HandlesAlignedArgsSize) {
  EXPECT_EQ(traits::aligned_args_size_to_stack_v<void(mywr::byte_t)>,
            detail::kStackAlignment);
  EXPECT_EQ(traits::aligned_args_size_to_stack_v<void(int)>,
            detail::kStackAlignment);
}
