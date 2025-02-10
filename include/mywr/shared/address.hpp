/**
 * @file address.hpp
 * @author themusaigen
 * @brief A module with a class for converting pointers to integers.
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MYWR_SHARED_ADDRESS_HPP_
#define MYWR_SHARED_ADDRESS_HPP_

#include <cstddef>
#include <shared/core.hpp>

#include <type_traits>
#include <utility>

/**
 * @brief The core namespace of the `mywr` library.
 */
namespace mywr {
/**
 * @brief A utility class that converts incoming pointers to integer types
 * (address).
 * @details A utility class that, when constructed from a pointer, converts it
 * to an integer type (address), otherwise, when constructed from an integer
 * value, it retains this value. The advantage of the class is that it can be
 * used as a parameter type in a function, meaning you no longer need to write
 * overloads for void*, uint32/64_t, and so on.
 */
class address {
  /**
   * @brief The address of a passed pointer, or integer value.
   */
  mywr::address_t m_address;

public:
  address()               = default;
  ~address()              = default;
  address(const address&) = default;

  address(address&& rhs) noexcept
      : m_address(std::exchange(rhs.m_address, 0)) {}

  address(std::nullptr_t) noexcept
      : m_address(0) {}

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  address(T integral) noexcept
      : m_address(static_cast<mywr::address_t>(integral)) {}

  template<typename T>
  address(T* pointer) noexcept
      : m_address(reinterpret_cast<mywr::address_t>(pointer)) {}

  /**
   * @brief Converts an integer value to a specified integer or floating-point
   * type.
   */
  template<typename T = mywr::address_t,
           typename   = std::enable_if_t<std::is_integral_v<T> ||
                                         std::is_floating_point_v<T>>>
  [[nodiscard]] auto value() const noexcept {
    return static_cast<std::remove_cv_t<T>>(m_address);
  }

  /**
   * @brief Converts an integer value to a specified pointer type.
   */
  template<typename T>
  [[nodiscard]] auto pointer() const noexcept {
    // NOLINTBEGIN(*-no-int-to-ptr)
    return reinterpret_cast<std::remove_cv_t<T>*>(m_address);
    // NOLINTEND(*-no-int-to-ptr)
  }

  /**
   * @brief Checks whether this address is valid by converting it to void* and
   * comparing it to nullptr.
   *
   * @return true Valid.
   * @return false Invalid.
   */
  [[nodiscard]] auto valid() const noexcept -> bool {
    return pointer<void>() != nullptr;
  }

  /* ---------------------- Assignment operator overloads ---------------------
   */

  auto operator=(const address&) noexcept -> address& = default;
  auto operator=(address&& rhs) noexcept -> address& {
    m_address = std::exchange(rhs.m_address, 0);
    return *this;
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  auto operator=(T integral) noexcept -> address& {
    m_address = static_cast<mywr::address_t>(integral);
    return *this;
  }

  template<typename T>
  auto operator=(T* pointer) noexcept -> address& {
    m_address = reinterpret_cast<mywr::address_t>(pointer);
    return *this;
  }

  /* ---------------------- Logical operator overloads --------------------- */

  operator bool() const noexcept {
    return valid();
  }

  operator mywr::address_t() const noexcept {
    return m_address;
  }

  template<typename T,
           typename = std::enable_if_t<std::is_integral_v<T> ||
                                       std::is_floating_point_v<T>>>
  operator T() const noexcept {
    return value<T>();
  }

  template<typename T>
  operator T*() const noexcept {
    return pointer<T>();
  }

  /* ---------------------- Arithmetic operator overloads ---------------------
   */

  auto operator++() noexcept -> address& {
    m_address++;
    return *this;
  }

  auto operator++(int) noexcept -> address {
    auto temp = *this;
    ++*this;
    return temp;
  }

  auto operator--() noexcept -> address& {
    m_address--;
    return *this;
  }

  auto operator--(int) noexcept -> address {
    auto temp = *this;
    --*this;
    return temp;
  }

  auto operator+=(const address& rhs) noexcept -> address& {
    *this = *this + rhs;
    return *this;
  }

  auto operator-=(const address& rhs) noexcept -> address& {
    *this = *this - rhs;
    return *this;
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  auto operator+=(const T& rhs) noexcept -> address& {
    *this = *this + rhs;
    return *this;
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  auto operator-=(const T& rhs) noexcept -> address& {
    *this = *this - rhs;
    return *this;
  }

  friend auto operator+(const address& lhs, const address& rhs) noexcept
      -> address {
    return {lhs.value() + rhs.value()};
  }

  friend auto operator-(const address& lhs, const address& rhs) noexcept
      -> address {
    return {lhs.value() - rhs.value()};
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator+(const T& lhs, const address& rhs) noexcept -> address {
    return {lhs + rhs.value()};
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator+(const address& lhs, const T& rhs) noexcept -> address {
    return {lhs.value() + rhs};
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator-(const T& lhs, const address& rhs) noexcept -> address {
    return {lhs - rhs.value()};
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator-(const address& lhs, const T& rhs) noexcept -> address {
    return {lhs.value() - rhs};
  }

  /* ---------------------- Comparison operator overloads ---------------------
   */

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator>(const T& lhs, const address& rhs) noexcept -> bool {
    return lhs > rhs.value();
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator>(const address& lhs, const T& rhs) noexcept -> bool {
    return lhs.value() > rhs;
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator>=(const T& lhs, const address& rhs) noexcept -> bool {
    return lhs >= rhs.value();
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator>=(const address& lhs, const T& rhs) noexcept -> bool {
    return lhs.value() >= rhs;
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator<(const T& lhs, const address& rhs) noexcept -> bool {
    return lhs < rhs.value();
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator<(const address& lhs, const T& rhs) noexcept -> bool {
    return lhs.value() < rhs;
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator<=(const T& lhs, const address& rhs) noexcept -> bool {
    return lhs <= rhs.value();
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator<=(const address& lhs, const T& rhs) noexcept -> bool {
    return lhs.value() <= rhs;
  }

  friend auto operator>(const address& lhs, const address& rhs) noexcept
      -> bool {
    return lhs.value() > rhs.value();
  }

  friend auto operator<(const address& lhs, const address& rhs) noexcept
      -> bool {
    return lhs.value() < rhs.value();
  }

  friend auto operator>=(const address& lhs, const address& rhs) noexcept
      -> bool {
    return lhs.value() >= rhs.value();
  }

  friend auto operator<=(const address& lhs, const address& rhs) noexcept
      -> bool {
    return lhs.value() <= rhs.value();
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator==(const T& lhs, const address& rhs) noexcept -> bool {
    return lhs == rhs.value();
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator==(const address& lhs, const T& rhs) noexcept -> bool {
    return lhs.value() == rhs;
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator!=(const T& lhs, const address& rhs) noexcept -> bool {
    return lhs != rhs.value();
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  friend auto operator!=(const address& lhs, const T& rhs) noexcept -> bool {
    return lhs.value() != rhs;
  }

  friend auto operator==(const address& lhs, const address& rhs) noexcept
      -> bool {
    return lhs.value() == rhs.value();
  }

  friend auto operator!=(const address& lhs, const address& rhs) noexcept
      -> bool {
    return lhs.value() != rhs.value();
  }
};
} // namespace mywr

#endif
