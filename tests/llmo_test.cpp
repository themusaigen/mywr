#include <gtest/gtest.h>

#include "mywr/mywr.hpp"

namespace llmo = mywr::llmo;

TEST(LLMOTest, ShouldProcessDefaultLLMOperations) {
  int value = 2;
  int copyValue = 90;
  int cmpValue = 24;

  ASSERT_EQ(llmo::read<int>(&value), 2);

  llmo::write<int>(&value, 123);

  ASSERT_EQ(value, 123);

  // 0xc0000005 if one of args is not an object (except size).
  llmo::copy(&value, &copyValue, 1);

  ASSERT_EQ(value, 90);

  llmo::fill(&value, 24, 1);

  ASSERT_EQ(value, 24);

  // 0xc0000005 if one of args is not an object (except size).
  ASSERT_EQ(llmo::compare(&value, &cmpValue, 1), 0);
}
