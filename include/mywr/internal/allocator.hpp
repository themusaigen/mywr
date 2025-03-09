/**
 * @file allocator.hpp
 * @brief Defines memory allocation and management functionality for the mywr
 * library.
 *
 * @details
 * This header file provides declarations for memory allocation and deallocation
 * functions, as well as a RAII wrapper class for managing memory blocks. The
 * actual implementations of these functions are provided in platform-specific
 * header files, such as allocator.inl for Windows.
 *
 * The mywr::allocator namespace encapsulates the following key functionalities:
 * - Memory allocation with and without a specified address
 * - Memory deallocation
 * - A scoped_memory_block class for automatic memory management
 *
 * @note The implementation details are separated into platform-specific files,
 * which are included based on the target platform defined by preprocessor
 * macros.
 *
 * @author themusaigen
 */
#pragma once

#include <shared/core/address.hpp>
#include <shared/core/os.hpp>

#include <tl/expected.hpp>

/**
 * @brief Contains memory allocation and management functions for the mywr
 * library.
 */
namespace mywr::allocator {
/**
 * @brief Allocates a block of memory of the specified size.
 *
 * @param size The size of the memory block to allocate.
 * @return A tl::expected containing either the allocated memory address or an
 * error code.
 */
[[nodiscard]] inline auto allocate(size_t size) noexcept
    -> tl::expected<mywr::address, uint32_t>;

/**
 * @brief Allocates a block of memory of the specified size at the given
 * address.
 *
 * @param address The desired address for the memory block.
 * @param size The size of the memory block to allocate.
 * @return A tl::expected containing either the allocated memory address or an
 * error code.
 */
[[nodiscard]] inline auto allocate(const address& address, size_t size) noexcept
    -> tl::expected<mywr::address, uint32_t>;

/**
 * @brief Deallocates a block of memory at the specified address.
 *
 * @param address The address of the memory block to deallocate.
 * @return true if deallocation was successful, false otherwise.
 */
inline auto deallocate(const address& address) noexcept -> bool;

/**
 * @brief Deallocates a block of memory of the specified size at the given
 * address.
 *
 * @param address The address of the memory block to deallocate.
 * @param size The size of the memory block to deallocate.
 * @return true if deallocation was successful, false otherwise.
 */
inline auto deallocate(const address& address, size_t size) noexcept -> bool;

/**
 * @class scoped_memory_block
 * @brief RAII wrapper for managing memory blocks.
 *
 * This class provides automatic memory management for allocated memory blocks,
 * ensuring that the memory is properly deallocated when the object goes out of
 * scope.
 */
class scoped_memory_block {
private:
  address m_block{}; ///< The address of the allocated memory block.
  size_t  m_size{};  ///< The size of the allocated memory block.
  bool    m_allocated{
      false}; ///< Flag indicating if memory was successfully allocated.
  uint32_t m_errcode{}; ///< Error code in case allocation fails.

public:
  scoped_memory_block()                           = delete;
  scoped_memory_block(const scoped_memory_block&) = delete;

  /**
   * @brief Move constructor.
   * @param rhs The scoped_memory_block to move from.
   */
  scoped_memory_block(scoped_memory_block&& rhs) noexcept
      : m_block(std::move(rhs.m_block))
      , m_size(rhs.m_size)
      , m_allocated(rhs.m_allocated)
      , m_errcode(rhs.m_errcode) {
    rhs.m_allocated = false;
  }

  auto operator=(const scoped_memory_block&) noexcept
      -> scoped_memory_block& = delete;

  /**
   * @brief Move assignment operator.
   * @param rhs The scoped_memory_block to move from.
   * @return Reference to this object.
   */
  auto operator=(scoped_memory_block&& rhs) noexcept -> scoped_memory_block& {
    m_block     = std::move(rhs.m_block);
    m_size      = rhs.m_size;
    m_allocated = rhs.m_allocated;
    m_errcode   = rhs.m_errcode;

    rhs.m_allocated = false;
    return *this;
  }

  /**
   * @brief Constructor that allocates memory at a specified address.
   * @param address The desired address for the memory block.
   * @param size The size of the memory block to allocate.
   */
  scoped_memory_block(const address& address, size_t size) noexcept
      : m_size(size) {
    auto block = allocate(address, size);
    if (block.has_value()) {
      m_block     = *block;
      m_allocated = true;
    } else {
      m_errcode = block.error();
    }
  }

  /**
   * @brief Constructor that allocates memory of a specified size.
   * @param size The size of the memory block to allocate.
   */
  scoped_memory_block(size_t size) noexcept
      : scoped_memory_block(0, size) {}

  /**
   * @brief Destructor that automatically deallocates the memory block.
   */
  ~scoped_memory_block() {
    if (m_allocated) {
      deallocate(m_block, m_size);
    }
  }

  /**
   * @brief Get the address of the allocated memory block.
   * @return The address of the allocated memory block.
   */
  [[nodiscard]] auto get() const noexcept -> auto& {
    return m_block;
  }

  /**
   * @brief Get the size of the allocated memory block.
   * @return The size of the allocated memory block.
   */
  [[nodiscard]] auto size() const noexcept -> size_t {
    return m_size;
  }

  /**
   * @brief Check if memory was successfully allocated.
   * @return true if memory was allocated, false otherwise.
   */
  [[nodiscard]] auto allocated() const noexcept -> bool {
    return m_allocated;
  }

  /**
   * @brief Get the error code if allocation failed.
   * @return The error code if allocation failed, 0 otherwise.
   */
  [[nodiscard]] auto error() const noexcept -> uint32_t {
    return m_errcode;
  }
};
} // namespace mywr::allocator

#if defined(MYWR_WINDOWS)
  #include <internal/platform/windows/allocator.inl>
#endif
