#include <gtest/gtest.h>

#include <internal/hook.hpp>

#include "hook32.hpp"

using namespace mywr::hook;

// NOLINTBEGIN(*-convert-member-functions-to-static)
class A {
public:
  auto MYWR_THISCALL pod_thiscall(int lhs, int rhs) -> int {
    return lhs + rhs;
  }

  auto MYWR_THISCALL nonpod_thiscall() -> NonPod {
    return {1, 1, 1};
  }

  auto MYWR_THISCALL nonpod_thiscall_with_initializers(int param0,
                                                       int param1,
                                                       int param2) -> NonPod {
    return {param0, param1, param2};
  }
};
// NOLINTEND(*-convert-member-functions-to-static)

TEST(Hook, ThiscallPod) {
  constexpr auto param_1{2};
  constexpr auto param_2{2};
  constexpr auto multiplier{2};
  constexpr auto expected_result{(param_1 + param_2) * multiplier};

  A object;

  hook<decltype(&A::pod_thiscall)> pod_hook;
  pod_hook.target(&A::pod_thiscall);
  pod_hook.redirect(
      [&]([[maybe_unused]] const auto& hook, A* obj, int lhs, int rhs) -> int {
        return hook.call(obj, lhs, rhs) * multiplier;
      });

  auto result{pod_hook.install()};

  EXPECT_FALSE(result) << "Failed to install hook, error: "
                       << static_cast<int>(result.value());
  EXPECT_EQ(object.pod_thiscall(param_1, param_2), expected_result);
}

TEST(Hook, ThiscallNonPod) {
  constexpr auto expected_result{
      NonPod{0, 0, 0}
  };

  A object;

  hook<decltype(&A::nonpod_thiscall)> nonpod_hook;
  nonpod_hook.target(&A::nonpod_thiscall);
  nonpod_hook.redirect([&]([[maybe_unused]] const auto& hook,
                           [[maybe_unused]] A* obj) noexcept -> NonPod {
    return expected_result;
  });

  auto result{nonpod_hook.install()};

  EXPECT_FALSE(result) << "Failed to install hook, error: "
                       << static_cast<int>(result.value());
  EXPECT_EQ(object.nonpod_thiscall(), expected_result);
}

TEST(Hook, ThiscallNonPodWithInitializers) {
  constexpr auto param_1{1};
  constexpr auto param_2{2};
  constexpr auto param_3{3};
  constexpr auto expected_result{
      NonPod{0, 0, 0}
  };

  A object;

  hook<decltype(&A::nonpod_thiscall_with_initializers)> nonpod_hook;
  nonpod_hook.target(&A::nonpod_thiscall_with_initializers);
  nonpod_hook.redirect([&]([[maybe_unused]] const auto& hook,
                           [[maybe_unused]] A*          obj,
                           [[maybe_unused]] int         param0,
                           [[maybe_unused]] int         param1,
                           [[maybe_unused]] int         param2) -> NonPod {
    return expected_result;
  });

  auto result{nonpod_hook.install()};

  EXPECT_FALSE(result) << "Failed to install hook, error: "
                       << static_cast<int>(result.value());
  EXPECT_EQ(object.nonpod_thiscall_with_initializers(param_1, param_2, param_3),
            expected_result);
}
