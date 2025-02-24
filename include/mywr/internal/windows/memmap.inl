/**
 * @file memmap.inl
 * @brief Windows-specific implementation of memory mapping functions.
 *
 * This file contains inline functions for querying the state of memory pages
 * on Windows systems. It is part of the mywr::memmap namespace and provides
 * functionality to check if memory pages are occupied, free, or reserved.
 *
 * @note This file is specific to Windows and uses the Windows API.
 *
 * @author themusaigen
 */
#pragma once

#include <shared/core/osheaders.hpp>

#include <internal/memmap.hpp>

namespace mywr::memmap {
/**
 * @brief Checks if a memory page is occupied.
 *
 * This function queries the state of a memory page at the given base address
 * using the Windows API VirtualQuery function. It determines if the page
 * is currently in use (i.e., not free).
 *
 * @param base The base address of the memory page to check.
 * @return true if the memory page is occupied, false otherwise.
 * @note This function is marked as nodiscard to encourage checking its return
 * value.
 */
[[nodiscard]] inline auto is_memory_page_occupied(const address& base) noexcept
    -> bool {
  MEMORY_BASIC_INFORMATION mbi{};
  VirtualQuery(base, &mbi, sizeof(mbi));
  return mbi.State != MEM_FREE;
}

/**
 * @brief Checks if a memory page is free.
 *
 * This function is the logical inverse of is_memory_page_occupied. It checks
 * if the memory page at the given base address is free (i.e., not occupied).
 *
 * @param base The base address of the memory page to check.
 * @return true if the memory page is free, false otherwise.
 * @note This function is marked as nodiscard to encourage checking its return
 * value.
 */
[[nodiscard]] inline auto is_memory_page_free(const address& base) noexcept
    -> bool {
  return !is_memory_page_occupied(base);
}

/**
 * @brief Checks if a memory page is reserved.
 *
 * This function queries the state of a memory page at the given base address
 * using the Windows API VirtualQuery function. It determines if the page
 * is in a reserved state.
 *
 * @param base The base address of the memory page to check.
 * @return true if the memory page is reserved, false otherwise.
 * @note This function is marked as nodiscard to encourage checking its return
 * value.
 */
[[nodiscard]] inline auto is_memory_page_reserved(const address& base) noexcept
    -> bool {
  MEMORY_BASIC_INFORMATION mbi{};
  VirtualQuery(base, &mbi, sizeof(mbi));
  return (mbi.State & MEM_RESERVE) != 0;
}
} // namespace mywr::memmap
