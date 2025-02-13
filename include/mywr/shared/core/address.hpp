/**
 * @file address.hpp
 * @author themusaigen
 * @brief A module with a class for converting pointers to integers.
 *
 * This file contains the definition of the `address` class, which provides
 * utility functions for handling and converting between pointers and integer
 * representations of addresses.
 *
 * @copyright Copyright (c) 2025
 */
#pragma once

#include "core.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

/**
 * @brief The core namespace of the `mywr` library.
 */
namespace mywr {

/**
 * @class address
 * @brief A utility class that converts incoming pointers to integer types
 * (address).
 *
 * @details This class provides functionality to convert between pointers and
 * their integer representations. It can be constructed from either a pointer or
 * an integer value. The main advantage of this class is that it can be used as
 * a parameter type in a function, eliminating the need for separate overloads
 * for void*, uint32/64_t, etc.
 */
class address {
private:
  /**
   * @brief The address of a passed pointer, or integer value.
   */
  mywr::address_t m_address;

public:
  /**
   * @brief Default constructor.
   */
  address() noexcept = default;

  /**
   * @brief Default destructor.
   */
  ~address() noexcept = default;

  /**
   * @brief Default copy constructor.
   */
  constexpr address(const address&) noexcept = default;

  /**
   * @brief Move constructor.
   * @param rhs The address object to move from.
   */
  address(address&& rhs) noexcept
      : m_address(std::exchange(rhs.m_address, 0)) {}

  /**
   * @brief Constructor from nullptr.
   */
  constexpr address(std::nullptr_t) noexcept
      : m_address(0) {}

  /**
   * @brief Constructor from integral types.
   * @tparam T An integral type.
   * @param integral The integral value to construct from.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  constexpr address(T integral) noexcept
      : m_address(static_cast<address_t>(integral)) {
    static_assert(sizeof(T) <= sizeof(address_t),
                  "Integral type is too large for address_t");
  }

  /**
   * @brief Constructor from pointer types.
   * @tparam T Any type.
   * @param pointer The pointer to construct from.
   */
  template<typename T>
  constexpr address(T* pointer) noexcept
      : m_address(reinterpret_cast<address_t>(pointer)) {
    static_assert(sizeof(T*) == sizeof(address_t),
                  "Pointer size does not match address_t size");
  }

  /**
   * @brief Converts the stored address to a specified integer or floating-point
   * type.
   * @tparam T The type to convert to (defaults to address_t).
   * @return The converted value.
   */
  template<typename T = address_t,
           typename   = std::enable_if_t<std::is_integral_v<T> ||
                                         std::is_floating_point_v<T>>>
  [[nodiscard]] constexpr auto value() const noexcept {
    return static_cast<std::remove_cv_t<T>>(m_address);
  }

  /**
   * @brief Converts the stored address to a specified pointer type.
   * @tparam T The type to convert to.
   * @return The converted pointer.
   */
  template<typename T>
  [[nodiscard]] constexpr auto pointer() const noexcept {
    static_assert(sizeof(T*) == sizeof(address_t),
                  "Pointer size does not match address_t size");

    // NOLINTBEGIN(*-no-int-to-ptr)
    return reinterpret_cast<std::remove_cv_t<T>*>(m_address);
    // NOLINTEND(*-no-int-to-ptr)
  }

  /**
   * @brief Checks whether this address is valid.
   * @return true if the address is valid (not null), false otherwise.
   */
  [[nodiscard]] constexpr auto valid() const noexcept -> bool {
    return pointer<void>() != nullptr;
  }

  /* ---------------------- Assignment operator overloads ---------------------
   */

  /**
   * @brief Default copy assignment operator.
   */
  auto operator=(const address&) noexcept -> address& = default;

  /**
   * @brief Move assignment operator.
   * @param rhs The address object to move from.
   * @return Reference to this object.
   */
  auto operator=(address&& rhs) noexcept -> address& {
    m_address = std::exchange(rhs.m_address, 0);
    return *this;
  }

  /**
   * @brief Assignment operator for integral types.
   * @tparam T An integral type.
   * @param integral The integral value to assign.
   * @return Reference to this object.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  auto operator=(T integral) noexcept -> address& {
    m_address = static_cast<mywr::address_t>(integral);
    return *this;
  }

  /**
   * @brief Assignment operator for pointer types.
   * @tparam T Any type.
   * @param pointer The pointer to assign.
   * @return Reference to this object.
   */
  template<typename T>
  constexpr auto operator=(T* pointer) noexcept -> address& {
    m_address = reinterpret_cast<mywr::address_t>(pointer);
    return *this;
  }

  /* ---------------------- Logical operator overloads --------------------- */

  /**
   * @brief Boolean conversion operator.
   * @return true if the address is valid, false otherwise.
   */
  constexpr operator bool() const noexcept {
    return valid();
  }

  /**
   * @brief Conversion operator to mywr::address_t.
   * @return The stored address value.
   */
  constexpr operator mywr::address_t() const noexcept {
    return m_address;
  }

  /**
   * @brief Conversion operator to integral or floating-point types.
   * @tparam T The type to convert to.
   * @return The converted value.
   */
  template<typename T,
           typename = std::enable_if_t<std::is_integral_v<T> ||
                                       std::is_floating_point_v<T>>>
  constexpr operator T() const noexcept {
    return value<T>();
  }

  /**
   * @brief Conversion operator to pointer types.
   * @tparam T The type to convert to.
   * @return The converted pointer.
   */
  template<typename T>
  constexpr operator T*() const noexcept {
    return pointer<T>();
  }

  /* ---------------------- Arithmetic operator overloads ---------------------
   */

  /**
   * @brief Pre-increment operator.
   * @return Reference to this object after incrementing.
   */
  constexpr auto operator++() noexcept -> address& {
    m_address++;
    return *this;
  }

  /**
   * @brief Post-increment operator.
   * @return Copy of this object before incrementing.
   */
  auto operator++(int) noexcept -> address {
    auto temp = *this;
    ++*this;
    return temp;
  }

  /**
   * @brief Pre-decrement operator.
   * @return Reference to this object after decrementing.
   */
  constexpr auto operator--() noexcept -> address& {
    m_address--;
    return *this;
  }

  /**
   * @brief Post-decrement operator.
   * @return Copy of this object before decrementing.
   */
  auto operator--(int) noexcept -> address {
    auto temp = *this;
    --*this;
    return temp;
  }

  /**
   * @brief Addition assignment operator.
   * @param rhs The address to add.
   * @return Reference to this object.
   */
  auto operator+=(const address& rhs) noexcept -> address& {
    *this = *this + rhs;
    return *this;
  }

  /**
   * @brief Subtraction assignment operator.
   * @param rhs The address to subtract.
   * @return Reference to this object.
   */
  auto operator-=(const address& rhs) noexcept -> address& {
    *this = *this - rhs;
    return *this;
  }

  /**
   * @brief Addition assignment operator for integral types.
   * @tparam T An integral type.
   * @param rhs The value to add.
   * @return Reference to this object.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  constexpr auto operator+=(const T& rhs) noexcept -> address& {
    m_address += static_cast<address_t>(rhs);
    return *this;
  }

  /**
   * @brief Subtraction assignment operator for integral types.
   * @tparam T An integral type.
   * @param rhs The value to subtract.
   * @return Reference to this object.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  constexpr auto operator-=(const T& rhs) noexcept -> address& {
    m_address -= static_cast<address_t>(rhs);
    return *this;
  }

  /**
   * @brief Addition operator.
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return New address object with the sum.
   */
  friend constexpr auto operator+(const address& lhs,
                                  const address& rhs) noexcept -> address {
    return {lhs.value() + rhs.value()};
  }

  /**
   * @brief Subtraction operator.
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return New address object with the difference.
   */
  friend constexpr auto operator-(const address& lhs,
                                  const address& rhs) noexcept -> address {
    return {lhs.value() - rhs.value()};
  }

  /**
   * @brief Modulo operator.
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return New address object with the modulo result.
   */
  friend constexpr auto operator%(const address& lhs,
                                  const address& rhs) noexcept -> address {
    return {lhs.value() % rhs.value()};
  }

  /**
   * @brief Modulo assignment operator.
   *
   * Updates the current address with the modulo result of the current address
   * and the given address.
   *
   * @param rhs The address to perform modulo operation with.
   * @return Reference to this object after the assignment.
   */
  constexpr auto operator%=(const address& rhs) noexcept -> address& {
    m_address %= rhs.value();
    return *this;
  }

  /**
   * @brief Modulo assignment operator for integral types.
   *
   * Updates the current address with the modulo result of the current address
   * and the given integral value.
   *
   * @param rhs The integral value to perform modulo operation with.
   * @return Reference to this object after the assignment.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  constexpr auto operator%=(const T& rhs) noexcept -> address& {
    m_address %= static_cast<address_t>(rhs);
    return *this;
  }

  /**
   * @brief Bitwise AND operator.
   *
   * Performs a bitwise AND operation on the current address and the given
   * address.
   *
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return New address object with the result of the bitwise AND operation.
   */
  friend constexpr auto operator&(const address& lhs,
                                  const address& rhs) noexcept -> address {
    return {lhs.value() & rhs.value()};
  }

  /**
   * @brief Bitwise AND assignment operator for integral types.
   *
   * Updates the current address with the result of a bitwise AND operation with
   * the given integral value.
   *
   * @param rhs The integral value to perform bitwise AND operation with.
   * @return Reference to this object after the assignment.
   */
  constexpr auto operator&=(const address& rhs) noexcept -> address& {
    m_address &= rhs.value();
    return *this;
  }

  /**
   * @brief Bitwise AND assignment operator for integral types.
   *
   * Updates the current address with the result of a bitwise AND operation with
   * the given integral value.
   *
   * @param rhs The integral value to perform bitwise AND operation with.
   * @return Reference to this object after the assignment.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  constexpr auto operator&=(const T& rhs) noexcept -> address& {
    m_address &= static_cast<address_t>(rhs);
    return *this;
  }

  /**
   * @brief Bitwise OR operator.
   *
   * Performs a bitwise OR operation on the current address and the given
   * address.
   *
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return New address object with the result of the bitwise OR operation.
   */
  friend constexpr auto operator|(const address& lhs,
                                  const address& rhs) noexcept -> address {
    return {lhs.value() | rhs.value()};
  }

  /**
   * @brief Bitwise OR assignment operator.
   *
   * Updates the current address with the result of a bitwise OR operation with
   * the given address.
   *
   * @param rhs The address to perform bitwise OR operation with.
   * @return Reference to this object after the assignment.
   */
  constexpr auto operator|=(const address& rhs) noexcept -> address& {
    m_address |= rhs.value();
    return *this;
  }

  /**
   * @brief Bitwise OR assignment operator for integral types.
   *
   * Updates the current address with the result of a bitwise OR operation with
   * the given integral value.
   *
   * @param rhs The integral value to perform bitwise OR operation with.
   * @return Reference to this object after the assignment.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  constexpr auto operator|=(const T& rhs) noexcept -> address& {
    m_address |= static_cast<address_t>(rhs);
    return *this;
  }

  /**
   * @brief Bitwise XOR operator.
   *
   * Performs a bitwise XOR operation on the current address and the given
   * address.
   *
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return New address object with the result of the bitwise XOR operation.
   */
  friend constexpr auto operator^(const address& lhs,
                                  const address& rhs) noexcept -> address {
    return {lhs.value() ^ rhs.value()};
  }

  /**
   * @brief Bitwise XOR assignment operator.
   *
   * Updates the current address with the result of a bitwise XOR operation with
   * the given address.
   *
   * @param rhs The address to perform bitwise XOR operation with.
   * @return Reference to this object after the assignment.
   */
  constexpr auto operator^=(const address& rhs) noexcept -> address& {
    m_address ^= rhs.value();
    return *this;
  }

  /**
   * @brief Bitwise XOR assignment operator for integral types.
   *
   * Updates the current address with the result of a bitwise XOR operation with
   * the given integral value.
   *
   * @param rhs The integral value to perform bitwise XOR operation with.
   * @return Reference to this object after the assignment.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  constexpr auto operator^=(const T& rhs) noexcept -> address& {
    m_address ^= static_cast<address_t>(rhs);
    return *this;
  }

  /**
   * @brief Bitwise NOT operator.
   *
   * Performs a bitwise NOT operation on the current address.
   *
   * @param lhs The address to perform bitwise NOT operation on.
   * @return New address object with the result of the bitwise NOT operation.
   */
  friend constexpr auto operator~(const address& lhs) noexcept -> address {
    return {~lhs.value()};
  }

  /**
   * @brief Left shift assignment operator.
   *
   * Updates the current address with the result of a left shift operation with
   * the given address.
   *
   * @param rhs The address to perform left shift operation with.
   * @return Reference to this object after the assignment.
   */
  constexpr auto operator<<=(const address& rhs) noexcept -> address& {
    m_address <<= rhs.value();
    return *this;
  }

  /**
   * @brief Left shift assignment operator for integral types.
   *
   * Updates the current address with the result of a left shift operation with
   * the given integral value.
   *
   * @param rhs The integral value to perform left shift operation with.
   * @return Reference to this object after the assignment.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  constexpr auto operator<<=(const T& rhs) noexcept -> address& {
    m_address <<= static_cast<address_t>(rhs);
    return *this;
  }

  /**
   * @brief Right shift assignment operator.
   *
   * Updates the current address with the result of a right shift operation with
   * the given address.
   *
   * @param rhs The address to perform right shift operation with.
   * @return Reference to this object after the assignment.
   */
  constexpr auto operator>>=(const address& rhs) noexcept -> address& {
    m_address >>= rhs.value();
    return *this;
  }

  /**
   * @brief Right shift assignment operator for integral types.
   *
   * Updates the current address with the result of a right shift operation with
   * the given integral value.
   *
   * @param rhs The integral value to perform right shift operation with.
   * @return Reference to this object after the assignment.
   */
  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  constexpr auto operator>>=(const T& rhs) noexcept -> address& {
    m_address >>= static_cast<address_t>(rhs);
    return *this;
  }

  /**
   * @brief Left shift operator.
   *
   * Performs a left shift operation on the current address and the given
   * address.
   *
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return New address object with the result of the left shift operation.
   */
  friend constexpr auto operator<<(const address& lhs,
                                   const address& rhs) noexcept -> address {
    return {lhs.value() << rhs.value()};
  }

  /**
   * @brief Right shift operator.
   *
   * Performs a right shift operation on the current address and the given
   * address.
   *
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return New address object with the result of the right shift operation.
   */
  friend constexpr auto operator>>(const address& lhs,
                                   const address& rhs) noexcept -> address {
    return {lhs.value() >> rhs.value()};
  }

  /* ---------------------- Comparison operator overloads ---------------------
   */

  /**
   * @brief Equality comparison operator.
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return true if equal, false otherwise.
   */
  friend constexpr auto operator==(const address& lhs,
                                   const address& rhs) noexcept -> bool {
    return lhs.value() == rhs.value();
  }

  /**
   * @brief Inequality comparison operator.
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return true if not equal, false otherwise.
   */
  friend constexpr auto operator!=(const address& lhs,
                                   const address& rhs) noexcept -> bool {
    return !(lhs == rhs);
  }

  /**
   * @brief Less than comparison operator.
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return true if lhs is less than rhs, false otherwise.
   */
  friend constexpr auto operator<(const address& lhs,
                                  const address& rhs) noexcept -> bool {
    return lhs.value() < rhs.value();
  }

  /**
   * @brief Greater than comparison operator.
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return true if lhs is greater than rhs, false otherwise.
   */
  friend constexpr auto operator>(const address& lhs,
                                  const address& rhs) noexcept -> bool {
    return lhs.value() > rhs.value();
  }

  /**
   * @brief Less than or equal to comparison operator.
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return true if lhs is less than or equal to rhs, false otherwise.
   */
  friend constexpr auto operator<=(const address& lhs,
                                   const address& rhs) noexcept -> bool {
    return lhs.value() <= rhs.value();
  }

  /**
   * @brief Greater than or equal to comparison operator.
   * @param lhs Left-hand side address.
   * @param rhs Right-hand side address.
   * @return true if lhs is greater than or equal to rhs, false otherwise.
   */
  friend constexpr auto operator>=(const address& lhs,
                                   const address& rhs) noexcept -> bool {
    return !(lhs < rhs);
  }
};

} // namespace mywr
