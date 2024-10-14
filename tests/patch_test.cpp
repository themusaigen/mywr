#include <gtest/gtest.h>

#include "mywr/mywr.hpp"

using namespace mywr::patch;

TEST(PatchTest, ProcessesScopedWrites) {
  int value = 24;

  {
    scoped_write<int> scoped{&value, 26};

    EXPECT_EQ(value, 26);
  }

  EXPECT_EQ(value, 24);
}
