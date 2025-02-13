#include <internal/patch.hpp>

#include <gtest/gtest.h>

TEST(Patch, MemoryPatchable) {
  using namespace mywr::patch;

  {
    const int expected_result{4};

    int value{2};

    auto patch{
        patch_builder::create().target(&value).replace_with({0x04}).pack()};

    ASSERT_TRUE(patch.install());
    EXPECT_EQ(value, expected_result);
  }

  {
    const int expected_result{4};

    int value{2};

    auto patch{patch_builder::create()
                   .target(&value)
                   .replace_with(expected_result)
                   .pack()};

    ASSERT_TRUE(patch.install());
    EXPECT_EQ(value, expected_result);
  }

  {
    const char* expected_result = "Hello mywr!\0";
    const char* data            = "Hello world!";

    auto patch{patch_builder::create()
                   .target(&data)
                   .replace_with(expected_result)
                   .pack()};

    ASSERT_TRUE(patch.install());
    EXPECT_STREQ(data, expected_result);
  }

  {
    const int expected_result{4};
    const int expected_result_after_exiting_from_scope{1};

    int value{2};
    {
      auto patch{patch_builder::create()
                     .target(&value)
                     .replace_with(expected_result)
                     .with_original(expected_result_after_exiting_from_scope)
                     .pack()};

      ASSERT_TRUE(patch.install());
      EXPECT_EQ(value, expected_result);
    }

    EXPECT_EQ(value, expected_result_after_exiting_from_scope);
  }
}
