#include <gtest/gtest.h>

#include <internal/hook.hpp>

using namespace mywr::hook;

namespace {
auto MYWR_CDECL sum(int lhs, int rhs) -> int {
  return lhs + rhs;
}
} // namespace

TEST(Hook, Removable) {
  constexpr auto param_1{2};
  constexpr auto param_2{2};
  constexpr auto multiplier{2};
  constexpr auto expected_result{(param_1 + param_2) * multiplier};
  constexpr auto expected_result_without_hook{param_1 + param_2};

  hook<decltype(&sum)> sum_hook;
  sum_hook.target(&sum);
  sum_hook.redirect([&](const auto& hook, int lhs, int rhs) -> int {
    return hook.call(lhs, rhs) * multiplier;
  });

  {
    auto result{sum_hook.install()};

    EXPECT_FALSE(result) << "Failed to install hook, error: "
                         << static_cast<int>(result.value());
  }

  EXPECT_EQ(sum(param_1, param_2), expected_result);

  {
    auto result{sum_hook.remove()};

    EXPECT_FALSE(result) << "Failed to remove hook, error: "
                         << static_cast<int>(result.value());
  }

  EXPECT_EQ(sum(param_1, param_2), expected_result_without_hook);
}

TEST(Hook, Multiple) {
  constexpr auto param_1{2};
  constexpr auto param_2{2};
  constexpr auto multiplier{2};
  constexpr auto expected_result_without_hooks{param_1 + param_2};
  constexpr auto expected_result_with_one_hook{(param_1 + param_2) *
                                               multiplier};
  constexpr auto expected_result_with_two_hooks{
      ((param_1 + param_2) * multiplier) * multiplier};

  auto total_hook_calls{0};

  hook<decltype(&sum)> sum_hook;
  sum_hook.target(&sum);
  sum_hook.redirect([&](const auto& hook, int lhs, int rhs) -> int {
    ++total_hook_calls;
    return hook.call(lhs, rhs) * multiplier;
  });

  hook<decltype(&sum)> sum_hook2;
  sum_hook2.target(&sum);
  sum_hook2.redirect([&](const auto& hook, int lhs, int rhs) -> int {
    total_hook_calls += 2;
    return hook.call(lhs, rhs) * multiplier;
  });

  {
    auto result{sum_hook.install()};
    EXPECT_FALSE(result) << "Failed to install first hook, error: "
                         << static_cast<int>(result.value());
  }

  EXPECT_EQ(sum(param_1, param_2), expected_result_with_one_hook);
  EXPECT_EQ(total_hook_calls, 1);

  {
    auto result{sum_hook2.install()};
    EXPECT_FALSE(result) << "Failed to install second hook, error: "
                         << static_cast<int>(result.value());
  }

  EXPECT_EQ(sum(param_1, param_2), expected_result_with_two_hooks);
  EXPECT_EQ(total_hook_calls, 4);

  {
    auto result{sum_hook.remove()};
    EXPECT_FALSE(result) << "Failed to remove first hook, error: "
                         << static_cast<int>(result.value());
  }

  EXPECT_EQ(sum(param_1, param_2), expected_result_with_one_hook);
  EXPECT_EQ(total_hook_calls, 6);

  {
    auto result{sum_hook2.remove()};
    EXPECT_FALSE(result) << "Failed to remove second hook, error: "
                         << static_cast<int>(result.value());
  }

  EXPECT_EQ(sum(param_1, param_2), expected_result_without_hooks);
  EXPECT_EQ(total_hook_calls, 6);

  {
    auto result{sum_hook.install()};
    EXPECT_FALSE(result) << "Failed to reinstall first hook, error: "
                         << static_cast<int>(result.value());
  }

  EXPECT_EQ(sum(param_1, param_2), expected_result_with_one_hook);
  EXPECT_EQ(total_hook_calls, 7);

  {
    auto result{sum_hook2.install()};
    EXPECT_FALSE(result) << "Failed to reinstall second hook, error: "
                         << static_cast<int>(result.value());
  }

  EXPECT_EQ(sum(param_1, param_2), expected_result_with_two_hooks);
  EXPECT_EQ(total_hook_calls, 10);
}
