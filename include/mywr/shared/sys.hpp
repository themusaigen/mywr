/**
 * @file sys.hpp
 * @brief Provides system-related functions and constants.
 *
 * @details This header file is part of the `mywr` library. It defines
 * inline functions for retrieving the page size and allocation granularity,
 * which are essential for memory management and allocation strategies.
 *
 * The functions are implemented in platform-specific inline files, such as
 * `sys.inl` for Windows. The platform-specific implementation is included
 * based on the operating system macro defined in `os.hpp`.
 *
 * @author themusaigen
 */

#pragma once

#include <shared/core/os.hpp>

#include <cstdint>

/**
 * @brief Namespace containing system-related functions and utilities.
 *
 * @details The `mywr::sys` namespace provides a set of platform-independent
 * functions for retrieving system-specific information related to memory
 * management. These functions are essential for operations that require
 * knowledge of the underlying system's memory characteristics, such as
 * page size and allocation granularity.
 *
 * The functions in this namespace are designed to be used across different
 * platforms, with platform-specific implementations included based on the
 * operating system macros defined in `os.hpp`.
 *
 * Key features of this namespace include:
 * - Retrieval of system page size
 * - Retrieval of system allocation granularity
 *
 * These functions are particularly useful for low-level memory operations,
 * custom memory allocators, and other system-dependent tasks that require
 * precise knowledge of the underlying memory architecture.
 */
namespace mywr::sys {
/**
 * @brief Retrieves the page size in bytes.
 *
 * @details The page size is the size of a memory page, which is typically
 * used for memory allocation and deallocation. This function returns the
 * page size in bytes.
 *
 * @return The page size in bytes.
 */
inline auto get_page_size() noexcept -> uint32_t;

/**
 * @brief Retrieves the allocation granularity in bytes.
 *
 * @details The allocation granularity is the granularity at which memory
 * is allocated. It is typically used when allocating memory blocks within
 * a process. This function returns the allocation granularity in bytes.
 *
 * @return The allocation granularity in bytes.
 */
inline auto get_allocation_granularity() noexcept -> uint32_t;
} // namespace mywr::sys

#if defined(MYWR_WINDOWS)
  #include <shared/windows/sys.inl>
#endif
