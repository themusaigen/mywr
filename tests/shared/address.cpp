#include "shared/address.hpp"
#include "shared/core.hpp"

#include <gtest/gtest.h>

#include <cstdint>

TEST(Address, Constructible) {
  mywr::address addr{};

  ASSERT_EQ(addr.value(), 0);
}

// NOLINTBEGIN(*-magic-numbers)
TEST(Address, IntegralConstructible) {
  mywr::address addr0{static_cast<uint8_t>(0xFF)};
  mywr::address addr1{static_cast<uint16_t>(0xFFFF)};
  mywr::address addr2{static_cast<uint32_t>(0xFFFFFFFF)};

  ASSERT_EQ(addr0.value(), 0xFF);
  ASSERT_EQ(addr1.value(), 0xFFFF);
  ASSERT_EQ(addr2.value(), 0xFFFFFFFF);

#if defined(MYWR_64)
  mywr::address addr3{static_cast<uint64_t>(0xFFFFFFFFFFFFFFFFULL)};

  ASSERT_EQ(addr3.value(), 0xFFFFFFFFFFFFFFFFULL);
#endif
}
// NOLINTEND(*-magic-numbers)

TEST(Address, PointerConstructible) {
  uint8_t  some_value = 0x02;
  uint8_t* pointer    = &some_value;

  mywr::address addr0{pointer};
  mywr::address addr1{&some_value};

  ASSERT_EQ(addr0.value(), addr1.value());
}

TEST(Address, CopyConstructible) {
  mywr::address addr0{1};
  mywr::address addr1{addr0};

  ASSERT_EQ(addr0.value(), addr1.value());
}

TEST(Address, MoveConstructible) {
  mywr::address addr0{2};
  mywr::address addr1{std::move(addr0)};

  ASSERT_EQ(addr0.value(), 0);
  ASSERT_EQ(addr1.value(), 2);
}

TEST(Address, CopyAssignable) {
  mywr::address addr0{2};
  mywr::address addr1{};

  addr1 = addr0;

  ASSERT_EQ(addr0.value(), addr1.value());
}

TEST(Address, MoveAssignable) {
  mywr::address addr0{3};
  mywr::address addr1{};

  addr1 = std::move(addr0);

  ASSERT_EQ(addr0.value(), 0);
  ASSERT_EQ(addr1.value(), 3);
}

TEST(Address, Pointer) {
  uint8_t some_value = 0x01;

  mywr::address addr{&some_value};

  ASSERT_EQ(*addr.pointer<uint8_t>(), 0x01);
}

TEST(Address, IntegralAssignable) {
  mywr::address addr{};

  addr = 2;

  ASSERT_EQ(addr.value(), 2);
}

TEST(Address, PointerAssignable) {
  mywr::address addr{};

  uint8_t some_value = 0x02;

  addr = &some_value;

  ASSERT_EQ(addr.value(), reinterpret_cast<mywr::address_t>(&some_value));
}
