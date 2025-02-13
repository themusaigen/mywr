#include <internal/llmo.hpp>
#include <gtest/gtest.h>

TEST(Llmo, MemoryReadable) {
  {
    const auto expected_result{mywr::llmo::llmo_error::InvalidAddressError};
    const auto result{mywr::llmo::read<int>(nullptr)};

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), expected_result);
  }

  {
    const int expected_result{2};
    const int value{2};

    EXPECT_EQ(mywr::llmo::read<int>(&value).value(), expected_result);
  }
}

TEST(Llmo, MemoryWriteable) {
  {
    const auto expected_result{mywr::llmo::llmo_error::InvalidAddressError};
    const auto result{mywr::llmo::write<int>(nullptr, 0)};

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected_result);
  }

  {
    const int expected_result{4};
    const int value{2};

    EXPECT_FALSE(mywr::llmo::write<int>(&value, expected_result));
    EXPECT_EQ(value, expected_result);
  }
}

TEST(Llmo, MemoryCopyable) {
  {
    const auto expected_result{mywr::llmo::llmo_error::InvalidDestinationError};
    const auto result{mywr::llmo::copy(nullptr, nullptr, 1)};

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected_result);
  }

  {
    const auto value{2};
    const auto expected_result{mywr::llmo::llmo_error::InvalidDestinationError};
    const auto result{mywr::llmo::copy(&value, nullptr, 0)};

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected_result);
  }

  {
    const auto value{2};
    const auto expected_result{mywr::llmo::llmo_error::InvalidSourceError};
    const auto result{mywr::llmo::copy(&value, nullptr, 1)};

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected_result);
  }

  {
    const auto expected_result{2};
    const auto source_value{2};
    auto       dest_value{0};

    EXPECT_FALSE(mywr::llmo::copy(&dest_value, &source_value, sizeof(int)));
    EXPECT_EQ(dest_value, expected_result);
  }
}

TEST(Llmo, MemoryFillable) {
  {
    const auto expected_result{mywr::llmo::llmo_error::InvalidAddressError};
    const auto result{mywr::llmo::fill(nullptr, 0, 1)};

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected_result);
  }

  {
    const auto value{2};
    const auto expected_result{mywr::llmo::llmo_error::InvalidAddressError};
    const auto result{mywr::llmo::fill(&value, 0, 0)};

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected_result);
  }

  {
    const auto expected_result{4};

    auto value{2};

    EXPECT_FALSE(mywr::llmo::fill(&value, 4, 1));
    EXPECT_EQ(value, expected_result);
  }
}

TEST(Llmo, MemoryComparable) {
  {
    const auto expected_result{mywr::llmo::llmo_error::InvalidAddressError};
    const auto result{mywr::llmo::compare(nullptr, nullptr, 0)};

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), expected_result);
  }

  {
    const auto value{2};
    const auto expected_result{mywr::llmo::llmo_error::InvalidAddressError};
    const auto result{mywr::llmo::compare(&value, nullptr, 0)};

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), expected_result);
  }

  {
    const auto value{2};
    const auto expected_result{mywr::llmo::llmo_error::InvalidAddressError};
    const auto result{mywr::llmo::compare(&value, &value, 0)};

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), expected_result);
  }

  {
    const auto expected_result{0};
    const auto source_value{2};
    const auto dest_value{2};

    const auto result{
        mywr::llmo::compare(&dest_value, &source_value, sizeof(int))};

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected_result);
  }
}
