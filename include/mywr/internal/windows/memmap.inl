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

#include <algorithm>

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

// NOLINTBEGIN(*-swappable-parameters)
/**
 * @brief Finds the previous free memory page before the given page.
 *
 * This function searches for a free memory page in the range from the given
 * page to the minimum address, moving backwards in memory. The granularity
 * parameter specifies the alignment of memory pages.
 *
 * @param page The base address of the memory page to start searching from.
 * @param min_address The minimum address to search for a free memory page.
 * @param granularity The alignment of memory pages.
 * @return The base address of the previous free memory page, or 0 if no
 * free page is found.
 *
 * @note This function uses the VirtualQuery function to query memory page
 * information.
 */
[[nodiscard]] static auto find_prev_free_page(const address& page,
                                              const address& min_address,
                                              size_t granularity) noexcept
    -> address {
  // Initialize the starting address
  address address{page};

  // Align the starting address to the granularity
  address -= address.value() % granularity;

  // Move backwards in memory
  address -= granularity;

  // Search for a free memory page
  while (address >= min_address) {
    MEMORY_BASIC_INFORMATION mbi{};
    if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
      break;
    }

    // Check if the memory page is free
    if (mbi.State == MEM_FREE) {
      return address;
    }

    // Move to the next memory page
    if (reinterpret_cast<address_t>(mbi.AllocationBase) < granularity) {
      break;
    }
    address = reinterpret_cast<address_t>(mbi.AllocationBase) - granularity;
  }

  // No free memory page found
  return 0U;
}

/**
 * @brief Finds the next free memory page after the given page.
 *
 * This function searches for a free memory page in the range from the given
 * page to the maximum address, moving forwards in memory. The granularity
 * parameter specifies the alignment of memory pages.
 *
 * @param page The base address of the memory page to start searching from.
 * @param max_address The maximum address to search for a free memory page.
 * @param granularity The alignment of memory pages.
 * @return The base address of the next free memory page, or 0 if no free page
 * is found.
 *
 * @note This function uses the VirtualQuery function to query memory page
 * information.
 */
[[nodiscard]] static auto find_next_free_page(const address& page,
                                              const address& max_address,
                                              size_t granularity) noexcept
    -> address {
  // Initialize the starting address
  address address{page};

  // Align the starting address to the granularity
  address -= address.value() % granularity;

  // Move forwards in memory
  address += granularity;

  // Search for a free memory page
  while (address <= max_address) {
    MEMORY_BASIC_INFORMATION mbi{};
    if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
      break;
    }

    // Check if the memory page is free
    if (mbi.State == MEM_FREE) {
      return address;
    }

    // Move to the next memory page
    address = reinterpret_cast<address_t>(mbi.BaseAddress) + mbi.RegionSize;

    // Align the address to the granularity
    address += granularity - 1;
    address -= address.value() % granularity;
  }

  // No free memory page found
  return 0U;
}

// NOLINTEND(*-swappable-parameters)

/**
 * @brief Finds a free memory page within the specified range.
 *
 * This function searches for a free memory page within the range from the
 * given page to the specified range. The granularity parameter specifies the
 * alignment of memory pages.
 *
 * @param page The base address of the memory page to start searching from.
 * @param range The range within which to search for a free memory page.
 * @return The base address of the free memory page, or 0 if no free page is
 * found.
 *
 * @note This function uses the VirtualQuery function to query memory page
 * information and the find_prev_free_page and find_next_free_page functions
 * to find a free memory page.
 */
[[nodiscard]] static auto find_free_page(const address& page,
                                         size_t range) noexcept -> address {
  // Get system information
  SYSTEM_INFO sysinfo{};
  GetSystemInfo(&sysinfo);

  // Calculate the minimum and maximum addresses to search
  auto min_address =
      reinterpret_cast<address_t>(sysinfo.lpMinimumApplicationAddress);
  auto max_address =
      reinterpret_cast<address_t>(sysinfo.lpMaximumApplicationAddress);

  // Adjust the minimum address to the specified range
  auto address = address_t{page};
  if (address > range) {
    min_address = std::max(min_address, address - range);
  }

  // Adjust the maximum address to the specified range
  max_address = std::min(max_address, address + range);

  // Search for a free memory page before the given page
  address = find_prev_free_page(
      address, min_address, sysinfo.dwAllocationGranularity);
  if (address == 0U) {
    // Search for a free memory page after the given page
    return find_next_free_page(
        address_t{page}, max_address, sysinfo.dwAllocationGranularity);
  }

  // Return the found free memory page
  return address;
}
} // namespace mywr::memmap
