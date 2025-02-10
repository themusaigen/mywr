#include "shared/address.hpp"
#include "shared/core.hpp"

#include <gtest/gtest.h>

TEST(Address, Constructible) {
  {
    const mywr::address_t expected_result{0};
    const mywr::address   addr{nullptr};

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address_t expected_result{0};
    const mywr::address   addr{};

    ASSERT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address_t expected_result{1};
    const mywr::address   addr{expected_result};

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address_t  value{0};
    const mywr::address_t* ptr{&value};
    const mywr::address_t  expected_result{
        reinterpret_cast<mywr::address_t>(ptr)};
    const mywr::address addr{ptr};

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address addr0{1};
    const mywr::address addr1{addr0};

    EXPECT_EQ(addr0.value(), addr1.value());
  }

  {
    const mywr::address_t expected_result0{0};
    const mywr::address_t expected_result1{1};

    mywr::address addr0{1};
    mywr::address addr1{std::move(addr0)};

    EXPECT_EQ(addr0.value(), expected_result0);
    EXPECT_EQ(addr1.value(), expected_result1);
  }
}

TEST(Address, Assignable) {
  {
    const mywr::address_t expected_result{64};

    // Empty bro.
    mywr::address addr{};

    // Do assignment.
    addr = expected_result;

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address_t  value{};
    const mywr::address_t* ptr{&value};
    const mywr::address_t  expected_result{
        reinterpret_cast<mywr::address_t>(ptr)};

    mywr::address addr{};

    addr = ptr;

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    mywr::address addr0{1};
    mywr::address addr1{};

    addr1 = addr0;

    EXPECT_EQ(addr0.value(), addr1.value());
  }

  {
    const mywr::address_t expected_result0{0};
    const mywr::address_t expected_result1{1};

    mywr::address addr0{1};
    mywr::address addr1{};

    addr1 = std::move(addr0);

    EXPECT_EQ(addr0.value(), expected_result0);
    EXPECT_EQ(addr1.value(), expected_result1);
  }
}

TEST(Address, PrepostfixDecIncrementable) {
  {
    const mywr::address_t expected_result{1};

    mywr::address addr{};
    ++addr;

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address_t expected_result0{0};
    const mywr::address_t expected_result1{1};

    mywr::address addr{};

    EXPECT_EQ((addr++).value(), expected_result0);
    EXPECT_EQ(addr.value(), expected_result1);
  }

  {
    const mywr::address_t expected_result{0};

    mywr::address addr{1};

    --addr;

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address_t expected_result0{1};
    const mywr::address_t expected_result1{0};

    mywr::address addr{1};

    EXPECT_EQ((addr--).value(), expected_result0);
    EXPECT_EQ(addr.value(), expected_result1);
  }
}

TEST(Address, DecrementableIncrementable) {
  {
    const mywr::address_t expected_result{2};
    const mywr::address   addr0{1};
    const mywr::address   addr1{addr0};

    auto addr2 = addr1 + addr0;

    EXPECT_EQ(addr2.value(), expected_result);
  }

  {
    const mywr::address_t expected_result{0};
    const mywr::address   addr0{1};
    const mywr::address   addr1{addr0};

    auto addr2 = addr1 - addr0;

    EXPECT_EQ(addr2.value(), expected_result);
  }

  {
    const mywr::address_t expected_result{2};
    const mywr::address   addr0{1};

    auto addr1 = addr0 + 1U;
    auto addr2 = 1U + addr0;

    EXPECT_EQ(addr1.value(), expected_result);
    EXPECT_EQ(addr2.value(), expected_result);
  }

  {
    const mywr::address_t expected_result{0};
    const mywr::address   addr0{1};

    auto addr1 = addr0 - 1U;
    auto addr2 = 1U - addr0;

    EXPECT_EQ(addr1.value(), expected_result);
    EXPECT_EQ(addr2.value(), expected_result);
  }
}

TEST(Address, DecIncAssignable) {
  {
    const mywr::address_t expected_result{2};

    mywr::address addr{1};

    addr += addr;

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address_t expected_result{0};

    mywr::address addr{1};

    addr -= addr;

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address_t expected_result{2};

    mywr::address addr{1};

    addr += 1U;

    EXPECT_EQ(addr.value(), expected_result);
  }

  {
    const mywr::address_t expected_result{0};

    mywr::address addr{1};

    addr -= 1U;

    EXPECT_EQ(addr.value(), expected_result);
  }
}

TEST(Address, Comparable) {
  {
    const bool expected_result = true;

    const mywr::address addr{1};
    const mywr::address addr1{2};

    EXPECT_EQ(addr1 > addr, expected_result);
    EXPECT_EQ(addr1 < addr, !expected_result);
  }

  {
    const bool expected_result = true;

    const mywr::address addr{1};
    const mywr::address addr1{1};

    const mywr::address_t value{1};

    EXPECT_EQ(addr1 >= addr, expected_result);
    EXPECT_EQ(addr1 <= addr, expected_result);

    EXPECT_EQ(value >= addr, expected_result);
    EXPECT_EQ(addr >= value, expected_result);

    EXPECT_EQ(value <= addr, expected_result);
    EXPECT_EQ(addr <= value, expected_result);
  }

  {
    const bool expected_result = true;

    const mywr::address addr{1};
    const mywr::address addr1{1};
    const mywr::address addr2{2};

    const mywr::address_t value{1};
    const mywr::address_t value1{2};

    EXPECT_EQ(addr == addr1, expected_result);
    EXPECT_EQ(addr != addr2, expected_result);
    EXPECT_EQ(addr == addr2, !expected_result);

    EXPECT_EQ(addr == value, expected_result);
    EXPECT_EQ(value == addr, expected_result);

    EXPECT_EQ(addr == value1, !expected_result);
    EXPECT_EQ(value1 == addr, !expected_result);

    EXPECT_EQ(addr != value1, expected_result);
    EXPECT_EQ(value1 != addr, expected_result);
  }
}

TEST(Address, Validatable) {
  {
    const int  value = 1;
    const int* ptr   = &value;

    const mywr::address addr{};
    const mywr::address addr1{ptr};

    EXPECT_FALSE(addr.valid());
    EXPECT_TRUE(addr1.valid());
    EXPECT_FALSE(addr);
    EXPECT_TRUE(addr1);
  }
}

TEST(Address, ExplicitConvertable) {
  {
    const mywr::address_t expected_result{1};
    const mywr::address   addr{1};
    const mywr::address_t value{addr};

    EXPECT_EQ(value, expected_result);
  }

  {
    const mywr::address_t  value{64};
    const mywr::address_t* ptr0{&value};
    const mywr::address    addr{ptr0};
    const mywr::address_t* ptr1{addr};

    const mywr::address_t expected_address{
        reinterpret_cast<mywr::address_t>(ptr0)};
    const mywr::address_t expected_result{64};

    ASSERT_EQ(reinterpret_cast<mywr::address_t>(ptr1), expected_address);
    EXPECT_EQ(*ptr1, expected_result);
  }
}
