#include <gtest/gtest.h>

#include <shared/core/address.hpp>

using namespace mywr;

static_assert(sizeof(address_t) == sizeof(void*),
              "address_t should be the same size as a pointer");
static_assert(sizeof(address) == sizeof(address_t),
              "address should be the same size as address_t");

TEST(Address, DefaultConstruct) {
  const address_t expected_result{address_t{0}};
  const address   object{};

  EXPECT_EQ(object.value(), expected_result)
      << "Default constructed address should be 0, but was " << object.value();
}

TEST(Address, NullptrConstruct) {
  const address_t expected_result{0};
  const address   object{nullptr};

  EXPECT_EQ(object.value(), expected_result)
      << "Constructed from nullptr address should be 0, but was "
      << object.value();
}

TEST(Address, IntegerConstruct) {
  const address_t expected_result{1};
  const address   object{expected_result};

  EXPECT_EQ(object.value(), expected_result)
      << "Constructed from integer address should be " << expected_result
      << ", but was " << object.value();
}

TEST(Address, PointerConstruct) {
  const int       data{};
  const address_t expected_result{reinterpret_cast<address_t>(&data)};
  const address   object{&data};

  EXPECT_EQ(object.value(), expected_result)
      << "Constructed from pointer address should be " << expected_result
      << ", but was " << object.value();
}

TEST(Address, Copyable) {
  const address_t expected_result{1};
  const address   object_1{expected_result};
  const address   object_2{object_1};

  EXPECT_EQ(object_1.value(), expected_result)
      << "Copied address should be " << expected_result << ", but was "
      << object_1.value();
  EXPECT_EQ(object_2.value(), expected_result)
      << "Copied address should be " << expected_result << ", but was "
      << object_2.value();
}

TEST(Address, Moveable) {
  const address_t expected_result_for_object_1{0};
  const address_t expected_result_for_object_2{1};

  address object_1{expected_result_for_object_2};
  address object_2{std::move(object_1)};

  EXPECT_EQ(object_1.value(), expected_result_for_object_1)
      << "Moved-from address should be 0, but was " << object_1.value();
  EXPECT_EQ(object_2.value(), expected_result_for_object_2)
      << "Moved-to address should be " << expected_result_for_object_2
      << ", but was " << object_2.value();
}

TEST(Address, CopyAssignment) {
  const address_t expected_result{1};

  address object_1{expected_result};
  address object_2{0};

  object_2 = object_1;

  EXPECT_EQ(object_1.value(), expected_result)
      << "Copied-from address should be " << expected_result << ", but was "
      << object_1.value();
  EXPECT_EQ(object_2.value(), expected_result)
      << "Copied-to address should be " << expected_result << ", but was "
      << object_2.value();
}

TEST(Address, MoveAssignment) {
  const address_t expected_result_for_object_1{0};
  const address_t expected_result_for_object_2{1};

  address object_1{expected_result_for_object_2};
  address object_2{0};

  object_2 = std::move(object_1);

  EXPECT_EQ(object_1.value(), expected_result_for_object_1)
      << "Moved-from address should be 0, but was " << object_1.value();
  EXPECT_EQ(object_2.value(), expected_result_for_object_2)
      << "Moved-to address should be " << expected_result_for_object_2
      << ", but was " << object_2.value();
}

TEST(Address, IntegerAssignment) {
  const address_t expected_result{1};

  address object{0};

  object = expected_result;

  EXPECT_EQ(object.value(), expected_result)
      << "Assigned integer address should be " << expected_result
      << ", but was " << object.value();
}

TEST(Address, PointerAssignment) {
  const int       data{};
  const address_t expected_result{reinterpret_cast<address_t>(&data)};

  address object{0};

  object = &data;

  EXPECT_EQ(object.value(), expected_result)
      << "Assigned pointer address should be " << expected_result
      << ", but was " << object.value();
}

TEST(Address, Valid) {
  const address_t data_1{0x0000};
  const address_t data_2{0x1000};

  const address object_1{data_1};
  const address object_2{data_2};

  EXPECT_FALSE(object_1.valid());
  EXPECT_TRUE(object_2.valid());
}

TEST(Address, IntegerCast) {
  const address_t expected_result{1};
  const address   object{expected_result};

  EXPECT_EQ(address_t{object}, expected_result)
      << "Implicitly converted address should be " << expected_result
      << ", but was " << address_t{object};
}

TEST(Address, PointerCast) {
  const address_t  expected_result{1};
  const address_t  data{expected_result};
  const address    object{&data};
  const address_t* pointer{object};

  EXPECT_EQ(*pointer, expected_result);
}

TEST(Address, PreIncrement) {
  const address_t expected_result{1};

  address object{};

  EXPECT_EQ((++object).value(), expected_result);
}

TEST(Address, PostIncrement) {
  const address_t expected_result_pre{0};
  const address_t expected_result_post{1};

  address object{};

  EXPECT_EQ((object++).value(), expected_result_pre);
  EXPECT_EQ(object.value(), expected_result_post);
}

TEST(Address, PreDecrement) {
  const address_t expected_result{0};

  address object{1};

  EXPECT_EQ((--object).value(), expected_result);
}

TEST(Address, PostDecrement) {
  const address_t expected_result_pre{1};
  const address_t expected_result_post{0};

  address object{1};

  EXPECT_EQ((object--).value(), expected_result_pre);
  EXPECT_EQ(object.value(), expected_result_post);
}

TEST(Address, Addition) {
  const address_t expected_result{2};
  const address   object_1{1};
  const address   object_2{1};

  EXPECT_EQ((object_1 + object_2).value(), expected_result);
}

TEST(Address, Subtraction) {
  const address_t expected_result{0};
  const address   object_1{1};
  const address   object_2{1};

  EXPECT_EQ((object_1 - object_2).value(), expected_result);
}

TEST(Address, Modulo) {
  const address_t expected_result{1};
  const address   object_1{3};
  const address   object_2{2};

  EXPECT_EQ((object_1 % object_2).value(), expected_result);
}

TEST(Address, BitwiseAnd) {
  const address_t expected_result{1};
  const address   object_1{1};
  const address   object_2{1};

  EXPECT_EQ((object_1 & object_2).value(), expected_result);
}

TEST(Address, BitwiseOr) {
  const address_t expected_result{3};
  const address   object_1{3};
  const address   object_2{2};

  EXPECT_EQ((object_1 | object_2).value(), expected_result);
}

TEST(Address, BitwiseXor) {
  const address_t expected_result{1};
  const address   object_1{3};
  const address   object_2{2};

  EXPECT_EQ((object_1 ^ object_2).value(), expected_result);
}

TEST(Address, BitwiseNot) {
#if defined(MYWR_32)
  const address_t expected_result{0b11111111111111111111111111111100};
#elif defined(MYWR_64)
  const address_t expected_result{
      0b1111111111111111111111111111111111111111111111111111111111111100};
#endif
  const address object{3};

  EXPECT_EQ((~object).value(), expected_result);
}

TEST(Address, BitwiseLeftShift) {
  const address_t expected_result{0x06};
  const address   object_1{3};
  const address   object_2{1};

  EXPECT_EQ((object_1 << object_2).value(), expected_result);
}

TEST(Address, BitwiseRightShift) {
  const address_t expected_result{1};
  const address   object_1{3};
  const address   object_2{1};

  EXPECT_EQ((object_1 >> object_2).value(), expected_result);
}

TEST(Address, Equality) {
  const address object_1{0x0000};
  const address object_2{0x1000};

  EXPECT_FALSE(object_1 == object_2);
  EXPECT_TRUE(object_1 != object_2);
}
