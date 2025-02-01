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

#include <shared/core.hpp>

#include <type_traits>
#include <utility>

/**
 * @brief The core namespace of the `mywr` library.
 */
namespace mywr {
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

  auto operator=(const address&) -> address& = default;
  auto operator=(address&& rhs) noexcept -> address& {
    std::swap(rhs.m_address, m_address);
    return *this;
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  address(T integral)
      : m_address(static_cast<mywr::address_t>(integral)) {}

  template<typename T>
  address(T* pointer)
      : m_address(reinterpret_cast<mywr::address_t>(pointer)) {}

  template<typename T = mywr::address_t>
  [[nodiscard]] auto value() const -> T {
    return static_cast<T>(m_address);
  }

  template<typename T>
  [[nodiscard]] auto pointer() const -> T* {
    return reinterpret_cast<T*>(m_address);
  }

  template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
  auto operator=(T integral) -> address& {
    m_address = static_cast<mywr::address_t>(integral);
    return *this;
  }

  template<typename T>
  auto operator=(T* pointer) -> address& {
    m_address = reinterpret_cast<mywr::address_t>(pointer);
    return *this;
  }

  auto operator++() -> address& {
    m_address++;
    return *this;
  }

  auto operator++(int) -> address {
    auto temp = *this;
    ++*this;
    return temp;
  }

  auto operator--() -> address& {
    m_address--;
    return *this;
  }

  auto operator--(int) -> address {
    auto temp = *this;
    --*this;
    return temp;
  }
};
} // namespace mywr

#endif
