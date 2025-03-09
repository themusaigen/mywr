/**
 * @file sys.inl
 * @brief Provides platform-specific implementations for system-related
 * functions.
 *
 * @details This inline file is part of the `mywr` library. It contains the
 * platform-specific implementations for retrieving the page size and
 * allocation granularity on Windows. The functions are defined in the
 * `mywr::sys` namespace and are called by the corresponding functions in
 * `sys.hpp`.
 *
 * The `GetSystemInfo` function is used to retrieve the system information,
 * which includes the page size and allocation granularity. These values are
 * then returned by the inline functions.
 *
 * @author themusaigen
 */

#pragma once

#include <shared/sys.hpp>
#include <shared/core/osheaders.hpp>

namespace mywr::sys {
/**
 * @brief Retrieves the page size in bytes.
 *
 * @details This function retrieves the page size in bytes using the
 * `GetSystemInfo` function. The page size is the size of a memory page,
 * which is typically used for memory allocation and deallocation.
 *
 * @return The page size in bytes.
 */
inline auto get_page_size() noexcept -> uint32_t {
  SYSTEM_INFO sysinfo{};
  GetSystemInfo(&sysinfo);
  return sysinfo.dwPageSize;
}

/**
 * @brief Retrieves the allocation granularity in bytes.
 *
 * @details This function retrieves the allocation granularity in bytes using
 * the `GetSystemInfo` function. The allocation granularity is the granularity
 * at which memory is allocated. It is typically used when allocating memory
 * blocks within a process.
 *
 * @return The allocation granularity in bytes.
 */
inline auto get_allocation_granularity() noexcept -> uint32_t {
  SYSTEM_INFO sysinfo{};
  GetSystemInfo(&sysinfo);
  return sysinfo.dwAllocationGranularity;
}
} // namespace mywr::sys
