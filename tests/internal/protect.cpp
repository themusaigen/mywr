#include <gtest/gtest.h>

#include <internal/protect.hpp>

using namespace mywr::protect;

TEST(Protect, Acquirable) {
  {
    const auto expected_result{protection::ReadWrite};
    int        value{};

    EXPECT_EQ(get_protect(&value), expected_result);
  }

  {
    const auto expected_result{protection::ReadWrite};
    const int  value{};

    // Yeah, protection of constant variable is ReadWrite :)
    EXPECT_EQ(get_protect(&value), expected_result);
  }
}

TEST(Protect, Changeable) {
  {
    const auto expected_result_default{protection::ReadWrite};
    const auto expected_result_spoofed{protection::ReadWriteExecute};

    int value{};

    EXPECT_EQ(set_protect(&value, expected_result_spoofed, sizeof(int)),
              expected_result_default);
    EXPECT_EQ(set_protect(&value, expected_result_default, sizeof(int)),
              expected_result_spoofed);
  }

  {
    const auto expected_result_default{protection::ReadWrite};
    const auto expected_result_spoofed{protection::ReadWriteExecute};

    int value{};

    {
      // Protect must be automatically spoofed.
      scoped_protect xprotect{&value, expected_result_spoofed, sizeof(int)};

      // Check it.
      EXPECT_EQ(get_protect(&value), expected_result_spoofed);
    }

    // On destruction protection must be restored to default.
    EXPECT_EQ(get_protect(&value), expected_result_default);
  }
}
