/**
 * @file protect.hpp
 * @author themusaigen
 * @brief Module that contains internal protection utilities
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MYWR_INTERNAL_PROTECT_HPP_
#define MYWR_INTERNAL_PROTECT_HPP_

#include <shared/address.hpp>
#include <shared/protection_flags.hpp>

#include <cstddef>

namespace mywr::protect {
/**
 * @brief Gets the protect value for the specified memory area.
 */
[[nodiscard]] static auto get_protect(const mywr::address& address)
    -> protection_flags::Enum;

/**
 * @brief Sets the protect value for the specified memory area.
 */
static auto set_protect(const mywr::address&   address,
                        protection_flags::Enum new_protect,
                        std::size_t            size) -> protection_flags::Enum;

class scoped_protect {
  mywr::address          m_address;
  std::size_t            m_size;
  protection_flags::Enum m_protect;

public:
  scoped_protect()                      = delete;
  scoped_protect(const scoped_protect&) = delete;
  scoped_protect(scoped_protect&& rhs) noexcept
      : m_address(std::move(rhs.m_address))
      , m_size(rhs.m_size)
      , m_protect(rhs.m_protect) {}

  scoped_protect(mywr::address          address,
                 protection_flags::Enum protect,
                 std::size_t            size)
      : m_address(std::move(address))
      , m_size(size) {
    if (m_address.valid()) {
      m_protect = set_protect(m_address, protect, m_size);
    }
  }

  ~scoped_protect() {
    if (m_address.valid() && m_protect != protection_flags::None) {
      set_protect(m_address, m_protect, m_size);
    }
  }

  /* ---------------------- Assignment operator overloads ---------------------
   */

  auto operator=(const scoped_protect&) = delete;
  auto operator=(scoped_protect&& rhs) noexcept -> scoped_protect& {
    m_address = std::move(rhs.m_address);
    m_protect = rhs.m_protect;
    m_size    = rhs.m_size;
    return *this;
  }
};

} // namespace mywr::protect

#if defined(MYWR_WINDOWS)
  #include <internal/windows/protect.hpp>
#endif

#endif
