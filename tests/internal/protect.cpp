#include <internal/protect.hpp>
#include <gtest/gtest.h>

TEST(Protect, ProtectAcquirable) {
  {
    const auto expected_result{mywr::protect::protection::ReadWrite};
    int        value{};

    EXPECT_EQ(mywr::protect::get_protect(&value), expected_result);
  }

  {
    const auto expected_result{mywr::protect::protection::ReadWrite};
    const int  value{};

    // Yeah, protection of constant variable is ReadWrite :)
    EXPECT_EQ(mywr::protect::get_protect(&value), expected_result);
  }
}

TEST(Protect, ProtectChangeable) {
  {
    const auto expected_result_default{mywr::protect::protection::ReadWrite};
    const auto expected_result_spoofed{
        mywr::protect::protection::ReadWriteExecute};

    int value{};

    EXPECT_EQ(mywr::protect::set_protect(
                  &value, expected_result_spoofed, sizeof(int)),
              expected_result_default);
    EXPECT_EQ(mywr::protect::set_protect(
                  &value, expected_result_default, sizeof(int)),
              expected_result_spoofed);
  }

  {
    const auto expected_result_default{mywr::protect::protection::ReadWrite};
    const auto expected_result_spoofed{
        mywr::protect::protection::ReadWriteExecute};

    int value{};

    {
      // Protect must be automatically spoofed.
      mywr::protect::scoped_protect xprotect{
          &value, expected_result_spoofed, sizeof(int)};

      // Check it.
      EXPECT_EQ(mywr::protect::get_protect(&value), expected_result_spoofed);
    }

    // On destruction protection must be restored to default.
    EXPECT_EQ(mywr::protect::get_protect(&value), expected_result_default);
  }
}
