#include <gtest/gtest.h>

#include <internal/hook.hpp>

#include "hook32.hpp"

using namespace mywr::hook;

namespace {
auto MYWR_FASTCALL pod_fastcall(int lhs, int rhs) -> int {
  return lhs + rhs;
}

auto MYWR_FASTCALL nonpod_fastcall() -> NonPod {
  return {1, 1, 1};
}

auto MYWR_FASTCALL nonpod_fastcall_with_initializers(int param0,
                                                     int param1,
                                                     int param2) -> NonPod {
  return {param0, param1, param2};
}
} // namespace

TEST(Hook, FastcallPod) {
  constexpr auto param_1{2};
  constexpr auto param_2{2};
  constexpr auto multiplier{2};
  constexpr auto expected_result{(param_1 + param_2) * multiplier};

  hook<decltype(&pod_fastcall)> pod_hook;
  pod_hook.target(&pod_fastcall);
  pod_hook.redirect(
      [&]([[maybe_unused]] const auto& hook, int lhs, int rhs) -> int {
        return hook.call(lhs, rhs) * multiplier;
      });

  auto result{pod_hook.install()};

  EXPECT_FALSE(result) << "Failed to install hook, error: "
                       << static_cast<int>(result.value());
  EXPECT_EQ(pod_fastcall(param_1, param_2), expected_result);
}

TEST(Hook, FastcallNonPod) {
  constexpr auto expected_result{
      NonPod{0, 0, 0}
  };

  hook<decltype(&nonpod_fastcall)> nonpod_hook;
  nonpod_hook.target(&nonpod_fastcall);
  nonpod_hook.redirect(
      [&]([[maybe_unused]] const auto& hook) noexcept -> NonPod {
        return expected_result;
      });

  auto result{nonpod_hook.install()};

  EXPECT_FALSE(result) << "Failed to install hook, error: "
                       << static_cast<int>(result.value());
  EXPECT_EQ(nonpod_fastcall(), expected_result);
}

TEST(Hook, FastcallNonPodWithInitializers) {
  constexpr auto param_1{1};
  constexpr auto param_2{2};
  constexpr auto param_3{3};
  constexpr auto expected_result{
      NonPod{0, 0, 0}
  };

  hook<decltype(&nonpod_fastcall_with_initializers)> nonpod_hook;
  nonpod_hook.target(&nonpod_fastcall_with_initializers);
  nonpod_hook.redirect([&]([[maybe_unused]] const auto& hook,
                           [[maybe_unused]] int         param0,
                           [[maybe_unused]] int         param1,
                           [[maybe_unused]] int         param2) -> NonPod {
    return expected_result;
  });

  auto result{nonpod_hook.install()};

  EXPECT_FALSE(result) << "Failed to install hook, error: "
                       << static_cast<int>(result.value());
  EXPECT_EQ(nonpod_fastcall_with_initializers(param_1, param_2, param_3),
            expected_result);
}
