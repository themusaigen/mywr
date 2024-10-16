#include <gtest/gtest.h>

#include "mywr/mywr.hpp"

TEST(AddressTest, ShouldDefaultConstruct) {
  mywr::address addr{};

  ASSERT_EQ(addr.value(), static_cast<mywr::address_t>(0));
}

TEST(AddressTest, ShouldConstructFromIntegral) {
  ASSERT_NO_THROW(mywr::address{123});
  ASSERT_EQ(mywr::address{123}.value(), 123);
}

TEST(AddressTest, ShouldConstructFromPointer) {
  int  a   = 123;
  int* ptr = &a;

  ASSERT_NO_THROW(mywr::address{ptr});
  ASSERT_EQ(mywr::address{ptr}.value(), reinterpret_cast<mywr::address_t>(ptr));
}

TEST(AddressTest, ShouldConstructFromOtherAddress) {
  ASSERT_EQ(mywr::address{mywr::address{123}}.value(), 123);
  ASSERT_EQ(
      mywr::address{std::forward<mywr::address>(mywr::address{123})}.value(),
      123);
}

TEST(AddressTest, ShouldReturnProperlyValue) {
  auto address = mywr::address{123};

  ASSERT_EQ(address.value(), 123);
  ASSERT_EQ(static_cast<mywr::address_t>(address), 123);
}

TEST(AddressTest, ShouldMathOperate) {
  auto address  = mywr::address{123};
  auto address1 = mywr::address{123};

  ASSERT_EQ((address + address1).value(), 123 * 2);
  ASSERT_EQ((address - address1).value(), 0);

  address += address1;

  ASSERT_EQ(address.value(), 123 * 2);

  address -= address1;

  ASSERT_EQ(address.value(), 123);
}

TEST(AddressTest, ShouldLogicOperate) {
  auto address  = mywr::address{123};
  auto address1 = mywr::address{123};

  ASSERT_TRUE(address == address1);
  ASSERT_TRUE(address != mywr::address{124});
}

TEST(AddressTest, ShouldAssignOperate) {
  auto address = mywr::address{123};

  ASSERT_EQ(address.value(), 123);

  address = mywr::address{124};

  ASSERT_EQ(address.value(), 124);

  address = 125;

  ASSERT_EQ(address.value(), 125);
}
