/**
 * @file memmap.hpp
 * @brief Defines the memory mapping and management functionality for the mywr
 * library.
 *
 * @details
 * This header file provides the necessary declarations and function prototypes
 * for memory mapping and management operations in the mywr library. The actual
 * implementation of these functions is provided in platform-specific header
 * files, such as memmap.inl for Windows.
 *
 * The mywr::memmap namespace encapsulates the following key functionalities:
 * - Checking the status of memory pages (occupied, free, or reserved)
 * - Finding free memory pages (planned for future implementation)
 *
 * This modular approach allows for easy extension to support multiple platforms
 * while maintaining a consistent interface.
 *
 * @note The implementation details are separated into platform-specific files,
 * which are included based on the target platform defined by preprocessor
 * macros.
 *
 * @todo Implement functionality to find a free memory page.
 *
 * @author themusaigen
 */
#pragma once

#include <shared/core/os.hpp>
#include <shared/core/address.hpp>

/**
 * @brief Contains memory mapping and management functions for the mywr library.
 */
namespace mywr::memmap {
/**
 * @brief Checks if a memory page is occupied.
 *
 * @param base The base address of the memory page to check.
 * @return true if the memory page is occupied, false otherwise.
 * @note This function is implemented in platform-specific header files.
 */
[[nodiscard]] inline auto is_memory_page_occupied(const address& base) noexcept
    -> bool;

/**
 * @brief Checks if a memory page is free.
 *
 * @param base The base address of the memory page to check.
 * @return true if the memory page is free, false otherwise.
 * @note This function is implemented in platform-specific header files.
 */
[[nodiscard]] inline auto is_memory_page_free(const address& base) noexcept
    -> bool;

/**
 * @brief Checks if a memory page is reserved.
 *
 * @param base The base address of the memory page to check.
 * @return true if the memory page is reserved, false otherwise.
 * @note This function is implemented in platform-specific header files.
 */
[[nodiscard]] inline auto is_memory_page_reserved(const address& base) noexcept
    -> bool;

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
 * @return The base address of the previous free memory page, or 0 if no free
 * page is found.
 */
[[nodiscard]] static auto find_prev_free_page(const address& page,
                                              const address& min_address,
                                              size_t granularity) noexcept
    -> address;

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
 */
[[nodiscard]] static auto find_next_free_page(const address& page,
                                              const address& max_address,
                                              size_t granularity) noexcept
    -> address;

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
 */
[[nodiscard]] static auto find_free_page(const address& page,
                                         size_t range) noexcept -> address;
} // namespace mywr::memmap

#if defined(MYWR_WINDOWS)
  #include <internal/windows/memmap.inl>
#endif
