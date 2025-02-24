/**
 * @file allocator.inl
 * @brief Windows-specific implementation of memory allocation and deallocation
 * functions for the mywr library.
 *
 * @details
 * This inline header file provides the Windows-specific implementations of
 * memory allocation and deallocation functions declared in the allocator.hpp
 * file. It uses the Windows API functions VirtualAlloc and VirtualFree to
 * manage memory.
 *
 * @note This file is intended to be included only when compiling for Windows
 * platforms.
 *
 * @author themusaigen
 */
#pragma once

#include <shared/core/osheaders.hpp>

#include <internal/allocator.hpp>

#include <tl/expected.hpp>

namespace mywr::allocator {
/**
 * @brief Allocates a block of memory of the specified size.
 *
 * @param size The size of the memory block to allocate.
 * @return A tl::expected containing either the allocated memory address or an
 * error code.
 *
 * @details This function uses VirtualAlloc to allocate memory with read, write,
 * and execute permissions. If the allocation fails, it returns an unexpected
 * value with the last error code.
 */
[[nodiscard]] inline auto allocate(size_t size) noexcept
    -> tl::expected<mywr::address, uint32_t> {
  auto* block = VirtualAlloc(
      nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  if (block == nullptr) {
    return tl::make_unexpected(GetLastError());
  }

  return block;
}

/**
 * @brief Allocates a block of memory of the specified size at the given
 * address.
 *
 * @param address The desired address for the memory block.
 * @param size The size of the memory block to allocate.
 * @return A tl::expected containing either the allocated memory address or an
 * error code.
 *
 * @details This function uses VirtualAlloc to allocate memory at a specific
 * address with read, write, and execute permissions. If the allocation fails,
 * it returns an unexpected value with the last error code.
 */
[[nodiscard]] inline auto allocate(const address& address, size_t size) noexcept
    -> tl::expected<mywr::address, uint32_t> {
  auto* block = VirtualAlloc(address, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if (block == nullptr) {
    return tl::make_unexpected(GetLastError());
  }

  return block;
}

/**
 * @brief Deallocates a block of memory at the specified address.
 *
 * @param address The address of the memory block to deallocate.
 * @return true if deallocation was successful, false otherwise.
 *
 * @details This function uses VirtualFree to release the allocated memory.
 */
inline auto deallocate(const address& address) noexcept -> bool {
  return VirtualFree(address, 0, MEM_RELEASE) != 0;
}

/**
 * @brief Deallocates a block of memory of the specified size at the given
 * address.
 *
 * @param address The address of the memory block to deallocate.
 * @param size The size of the memory block to deallocate (unused in this
 * implementation).
 * @return true if deallocation was successful, false otherwise.
 *
 * @details This function uses VirtualFree to release the allocated memory.
 * The size parameter is marked as [[maybe_unused]] because VirtualFree doesn't
 * require it for MEM_RELEASE operations.
 */
inline auto deallocate(const address&          address,
                       [[maybe_unused]] size_t size) noexcept -> bool {
  return VirtualFree(address, 0, MEM_RELEASE) != 0;
}
} // namespace mywr::allocator
