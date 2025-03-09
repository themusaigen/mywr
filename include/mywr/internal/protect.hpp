/**
 * @file protect.hpp
 * @author themusaigen
 * @brief Header file providing utilities for managing memory protection.
 *
 * This header file defines a set of functions and a RAII class for managing
 * memory protection.  It allows for temporarily changing memory protection
 * flags (e.g., read, write, execute) and ensures that the original protection
 * is restored even in the presence of exceptions.  This is crucial for
 * operations that require direct memory manipulation but must prevent
 * unexpected behavior due to incorrect access permissions.
 *
 * The key component is the `scoped_protect` class, which provides a RAII
 * (Resource Acquisition Is Initialization) mechanism for managing temporary
 * protection changes.  Other functions offer ways to check and set protection
 * flags directly, providing flexibility for various scenarios.
 */
#pragma once

#include <shared/core/os.hpp>
#include <shared/core/address.hpp>
#include <shared/protection_flags.hpp>

#include <cstddef>

/**
 * @brief Namespace containing utilities for managing memory protection.
 *
 * @details This namespace provides tools for temporarily changing memory
 * protection flags and ensuring that the original protection is restored, even
 * in the presence of exceptions. It's designed for scenarios where direct
 * memory manipulation is necessary but requires careful management of memory
 * access permissions.
 *
 * The key component is the `scoped_protect` RAII class, which simplifies the
 * process of temporarily changing and restoring memory protection. Additional
 * functions provide ways to check and set protection flags directly.
 */
namespace mywr::protect {

/**
 * @brief Retrieves the memory protection flags for a specified memory address.
 *
 * @details This function retrieves the current memory protection flags for the
 * region of memory starting at the given address.
 *
 * @param address The starting address of the memory region. Must be a valid
 * address.
 * @return The memory protection flags for the specified address. Returns
 * `protection::None` if the address is invalid or if an error occurs.
 *
 * @note The function is declared as `[[nodiscard]]` to encourage the caller to
 * handle the returned value.
 */
[[nodiscard]] static auto get_protect(const address& address) -> protection;

/**
 * @brief Sets the memory protection flags for a specified memory region.
 *
 * @details This function sets the memory protection flags for a region of
 * memory starting at the given address and extending for the specified size. It
 * attempts to change the protection flags and returns the actual protection
 * flags after the change attempt.  This allows checking if the requested change
 * succeeded.
 *
 * @param address The starting address of the memory region. Must be a valid
 * address.
 * @param new_protect The desired memory protection flags.
 * @param size The size of the memory region in bytes.
 * @return The memory protection flags after the attempt to set them.  May
 * differ from `new_protect` if the change failed partially or entirely. Returns
 * `protection::None` if the address is invalid or if an error occurs.
 */
static auto set_protect(const address& address,
                        protection     new_protect,
                        size_t         size) -> protection;

/**
 * @brief Checks if a memory address is readable.
 *
 * @details This function checks the memory protection flags at the given
 * address to determine if the memory is readable.
 *
 * @param address The memory address to check.
 * @return `true` if the memory at the given address is readable; otherwise,
 * `false`.
 * @note The function uses bitwise AND to check the read permission bit.
 * @note The function is declared as `noexcept` indicating it will not throw
 * exceptions.
 */
[[nodiscard]] inline auto is_readable(const address& address) noexcept -> bool {
  return (get_protect(address) & protection::Read) == uint8_t(protection::Read);
}

/**
 * @brief Checks if a memory address is writable.
 *
 * @details This function checks the memory protection flags at the given
 * address to determine if the memory is writable.
 *
 * @param address The memory address to check.
 * @return `true` if the memory at the given address is writable; otherwise,
 * `false`.
 * @note The function uses bitwise AND to check the write permission bit.
 * @note The function is declared as `noexcept` indicating it will not throw
 * exceptions.
 */
[[nodiscard]] inline auto is_writeable(const address& address) noexcept
    -> bool {
  return (get_protect(address) & protection::Write) ==
         uint8_t(protection::Write);
}

/**
 * @brief Checks if a memory address is executable.
 *
 * @details This function checks the memory protection flags at the given
 * address to determine if the memory is executable.
 *
 * @param address The memory address to check.
 * @return `true` if the memory at the given address is executable; otherwise,
 * `false`.
 * @note The function uses bitwise AND to check the execute permission bit.
 * @note The function is declared as `noexcept` indicating it will not throw
 * exceptions.
 */
[[nodiscard]] inline auto is_executable(const address& address) noexcept
    -> bool {
  return (get_protect(address) & protection::Execute) ==
         uint8_t(protection::Execute);
}

/**
 * @brief A RAII class for managing temporary changes to memory protection.
 *
 * @details This class provides a mechanism for temporarily changing the
 * protection flags of a region of memory.  It ensures that the memory
 * protection is restored to its original state when the object goes out of
 * scope, even in the presence of exceptions.
 *
 * The constructor attempts to change the memory protection to the specified
 * protection flags. If successful, the destructor will restore the original
 * protection flags. If the constructor fails to change the protection, the
 * destructor will not attempt to restore it.
 */
class scoped_protect {
  /// The memory address to protect.
  address m_address;
  /// The size of the memory region to protect.
  size_t m_size;
  /// The original protection flags of the memory region.
  protection m_protect;

public:
  /// Default constructor is deleted as it's not meaningful to create an empty
  /// scoped_protect object.
  scoped_protect() = delete;
  /// Copy constructor is deleted to prevent copying.  scoped_protect objects
  /// should be moved only.
  scoped_protect(const scoped_protect&) = delete;
  /// Move constructor.
  scoped_protect(scoped_protect&& rhs) noexcept
      : m_address(std::move(rhs.m_address))
      , m_size(rhs.m_size)
      , m_protect(rhs.m_protect) {}

  /**
   * @brief Constructor that attempts to change the memory protection.
   *
   * @param address The memory address to protect.
   * @param protect The desired protection flags.
   * @param size The size of the memory region to protect.
   */
  scoped_protect(address address, protection protect, size_t size)
      : m_address(std::move(address))
      , m_size(size) {
    if (m_address.valid()) {
      m_protect = set_protect(m_address, protect, m_size);
    }
  }

  /// Destructor. Restores the original memory protection if necessary.
  ~scoped_protect() {
    if (valid()) {
      set_protect(m_address, m_protect, m_size);
    }
  }

  /**
   * @brief Indicates that the memory protection was successfully changed.
   * @return `true` if the memory protection was successfully changed;
   * otherwise, `false`.
   */
  [[nodiscard]] auto valid() const noexcept -> bool {
    return m_address.valid() && m_protect != protection::None;
  }

  /**
   * @brief Indicates that the memory protection was not successfully changed.
   * @return `true` if the memory protection was not successfully changed;
   * otherwise, `false`.
   */
  [[nodiscard]] auto invalid() const noexcept -> bool {
    return !valid();
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
  #include <internal/platform/windows/protect.inl>
#endif
