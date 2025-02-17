/**
 * @file llmo.hpp
 * @author themusaigen
 * @brief Header file providing low-level, platform-aware memory manipulation
 * functions.
 * @details This header file defines the `mywr::llmo` namespace, which contains
 * low-level memory operations. It offers safe and efficient methods for
 * reading, writing, copying, filling, and comparing memory regions. The library
 * incorporates error handling using `tl::expected` and optional memory
 * protection adjustments. Key features include:
 * - Exception-safe design
 * - Minimized risk of unexpected behavior due to memory access violations
 * - Platform-aware implementations (e.g., Windows-specific instruction cache
 * flushing)
 * - Optional memory protection changes for read/write operations
 * - CPU cache flushing after write operations
 *
 * The file includes necessary headers for OS-specific functionality, address
 * handling, and protection flags. It also uses the `protect.hpp` header for
 * memory protection operations.
 */
#pragma once

#include <shared/core/os.hpp>
#include <shared/core/osheaders.hpp>
#include <shared/core/address.hpp>
#include <shared/protection_flags.hpp>

#include "protect.hpp"

#include <tl/expected.hpp>

#include <type_traits>
#include <optional>

namespace mywr::detail {
// https://stackoverflow.com/a/39492671
template<typename T>
struct identity {
  using type = T;
};

template<typename T>
struct remove_all_pointers
    : std::conditional_t<std::is_pointer_v<T>,
                         remove_all_pointers<std::remove_pointer_t<T>>,
                         identity<T>> {};

template<typename T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;
} // namespace mywr::detail

/**
 * @brief The `mywr::llmo` namespace contains low-level memory operations.
 *
 * @details This namespace provides functions for reading, writing, copying, and
 * flushing memory, as well as managing memory protection. It is designed to be
 * efficient and flexible, allowing for direct manipulation of memory.
 */
namespace mywr::llmo {
/**
 * @brief An enum class representing error codes for low-level memory
 * operations.
 *
 */
enum class llmo_error : uint8_t {
  InvalidAddressError,
  InvalidProtectChangeError,
  UnreadableMemoryError,
  UnwriteableMemoryError,
  NullSizeError,

  InvalidDestinationError,
  InvalidSourceError
};

/**
 * @brief Flushes the instruction cache for the memory region specified by
 * `addr` and `size`.
 *
 * @details This function flushes the instruction cache for the memory region
 * specified by `addr` and `size`. It ensures that the CPU fetches the most
 * up-to-date instructions from memory. The implementation is conditional on the
 * platform:
 * - On Windows, it uses `FlushInstructionCache`.
 * - On other platforms, it returns `true` (effectively a no-op).
 *
 * @param addr The starting address of the memory region to flush. Must be a
 * valid address.
 * @param size The size of the memory region in bytes.
 * @return `true` if the instruction cache was successfully flushed or if the
 * operation is not supported on the current platform. `false` indicates an
 * error during the flush operation on Windows.
 * @note The `[[maybe_unused]]` attribute indicates that the parameters might
 * not be used on all platforms.
 * @warning The correctness and effectiveness of this function depend on the
 * underlying platform's cache management. Incorrect usage may not result in the
 * desired effect.
 */
inline auto flush([[maybe_unused]] const address& addr,
                  [[maybe_unused]] size_t         size) noexcept -> bool {
#if defined(MYWR_WINDOWS)
  return FlushInstructionCache(GetCurrentProcess(), addr, size) == 0;
#else
  return true;
#endif
}

/**
 * @brief Reads a value of type T from a given memory address.
 *
 * @details This function reads a value of type T from the specified memory
 * address. It optionally unprotects the memory region before reading, handling
 * potential errors such as invalid addresses, unreadable memory, and protection
 * changes.
 *
 * @tparam T The type of the value to read.
 * @param address The memory address to read from. Must be a valid address.
 * @param unprotect A boolean flag indicating whether to temporarily unprotect
 * the memory region before reading. Defaults to true. Setting this to false can
 * improve performance if the memory is already readable, but requires the
 * caller to ensure the memory is readable.
 * @return A tl::expected object containing the read value (Ok) or an llmo_error
 * (Err). Possible error codes include:
 *         - llmo_error::InvalidAddressError: The provided address is invalid.
 *         - llmo_error::UnreadableMemoryError: The memory at the specified
 * address is not readable. This error only occurs if `unprotect` is false.
 *         - llmo_error::InvalidProtectChangeError: An error occurred while
 * attempting to change memory protection. This error only occurs if `unprotect`
 * is true.
 *
 * @note The size of the read operation is determined by `sizeof(T)`.
 * @note The function uses `std::optional` for managing the `scoped_protect`
 * object, ensuring proper resource management.
 * @note The noexcept specification indicates that this function will not throw
 * exceptions. Error handling is done via the expected type.
 *
 * @code{.cpp}
 * // Example usage:
 * int value;
 * auto result = mywr::llmo::read<int>(&value);
 * if (result.has_value()) {
 *   // Access the value: result.value()
 * } else {
 *   // Handle the error: result.error()
 * }
 * @endcode
 */
template<typename T>
static auto read(const address& address, bool unprotect = true) noexcept
    -> tl::expected<T, llmo_error> {
  using namespace protect;

  static_assert(!std::is_void_v<detail::remove_all_pointers_t<T>>);

  if (!address) {
    return tl::make_unexpected(llmo_error::InvalidAddressError);
  }

  if (!unprotect && !is_readable(address)) {
    return tl::make_unexpected(llmo_error::UnreadableMemoryError);
  }

  std::optional<scoped_protect> protect;

  if (unprotect) {
    protect.emplace(address, protection::ReadWriteExecute, sizeof(T));

    if (protect->invalid()) {
      return tl::make_unexpected(llmo_error::InvalidProtectChangeError);
    }
  }

  return *address.pointer<T>();
}

/**
 * @brief Writes a value of type T to a given memory address.
 *
 * @details This function writes a value of type T to the specified memory
 * address. It optionally unprotects the memory region before writing, handling
 * potential errors such as invalid addresses, unwritable memory, and protection
 * changes.  The CPU cache is flushed after the write.
 *
 * @tparam T The type of the value to write.
 * @param address The memory address to write to. Must be a valid address.
 * @param value The value to write to the memory address.
 * @param unprotect A boolean flag indicating whether to temporarily unprotect
 * the memory region before writing. Defaults to true. Setting this to false can
 * improve performance if the memory is already writable, but requires the
 * caller to ensure the memory is writable.
 * @return A std::optional containing an llmo_error.  `std::nullopt` indicates
 * success; otherwise, it contains an error code. Possible error codes include:
 *         - llmo_error::InvalidAddressError: The provided address is invalid.
 *         - llmo_error::UnwriteableMemoryError: The memory at the specified
 * address is not writable. This error only occurs if `unprotect` is false.
 *         - llmo_error::InvalidProtectChangeError: An error occurred while
 * attempting to change memory protection. This error only occurs if `unprotect`
 * is true.
 *
 * @note The size of the write operation is determined by `sizeof(T)`.
 * @note The function uses `std::optional` for error handling and managing the
 * `scoped_protect` object, ensuring proper resource management.
 * @note The noexcept specification indicates that this function will not throw
 * exceptions. Error handling is done via the optional llmo_error return value.
 * @note The CPU cache is flushed after the write operation to ensure the data
 * is visible to other processes and cores.
 *
 * @code{.cpp}
 * // Example usage:
 * int value = 10;
 * auto error = mywr::llmo::write<int>(&value, 20);
 * if (!error) {
 *   // Write successful
 * } else {
 *   // Handle the error: error.value()
 * }
 * @endcode
 */
template<typename T>
static auto write(const address& address,
                  T              value,
                  bool unprotect = true) noexcept -> std::optional<llmo_error> {
  using namespace protect;

  static_assert(!std::is_void_v<detail::remove_all_pointers_t<T>>);

  if (!address) {
    return {llmo_error::InvalidAddressError};
  }

  if (!unprotect && !is_writeable(address)) {
    return {llmo_error::UnwriteableMemoryError};
  }

  std::optional<protect::scoped_protect> protect;

  if (unprotect) {
    protect.emplace(address, protect::protection::ReadWriteExecute, sizeof(T));

    if (protect->invalid()) {
      return {llmo_error::InvalidProtectChangeError};
    }
  }

  // Update the value.
  *address.pointer<T>() = value;

  // Flush the CPU cache.
  flush(address, sizeof(T));

  // No errors.
  return std::nullopt;
}

/**
 * @brief Copies a block of memory from a source address to a destination
 * address.
 *
 * @details This function copies `size` bytes of memory from the source address
 * (`src`) to the destination address (`dest`).  It optionally unprotects the
 * destination memory region before copying, handling potential errors such as
 * invalid addresses and protection changes. The CPU cache is flushed after the
 * copy.
 *
 * @param dest The destination memory address. Must be a valid address.
 * @param src The source memory address. Must be a valid address.
 * @param size The number of bytes to copy.
 * @param unprotect A boolean flag indicating whether to temporarily unprotect
 * the destination memory region before copying. Defaults to true. Setting this
 * to false can improve performance if the destination memory is already
 * writable, but requires the caller to ensure the destination memory is
 * writable.
 * @return An optional llmo_error. std::nullopt indicates success; otherwise,
 * it contains an error code. Possible error codes include:
 *         - llmo_error::InvalidDestinationError: The destination address is
 * invalid.
 *         - llmo_error::InvalidSourceError: The source address is invalid.
 *         - llmo_error::UnwriteableMemoryError: The destination memory at the
 * specified address is not writable. This error only occurs if `unprotect` is
 * false.
 *         - llmo_error::InvalidProtectChangeError: An error occurred while
 * attempting to change memory protection. This error only occurs if `unprotect`
 * is true.
 *         - llmo_error::NullSizeError: The size parameter is zero.
 *
 * @note The function uses std::optional for error handling and managing the
 * scoped_protect object, ensuring proper resource management.
 * @note The CPU cache is flushed after the copy operation to ensure the data is
 * visible to other processes and cores.
 *
 * @code{.cpp}
 * // Example usage:
 * char source[10] = "Hello";
 * char destination[10];
 * auto error = mywr::llmo::copy(destination, source, 10);
 * if (!error) {
 *   // Copy successful
 * } else {
 *   // Handle the error: error
 * }
 * @endcode
 */
static auto copy(const address& dest,
                 const address& src,
                 size_t         size,
                 bool unprotect = true) noexcept -> std::optional<llmo_error> {
  using namespace protect;

  if (size == 0) {
    return {llmo_error::NullSizeError};
  }

  if (!dest) {
    return {llmo_error::InvalidDestinationError};
  }

  if (!src) {
    return {llmo_error::InvalidSourceError};
  }

  if (!unprotect && !is_writeable(dest)) {
    return {llmo_error::UnwriteableMemoryError};
  }

  std::optional<protect::scoped_protect> protect;

  if (unprotect) {
    protect.emplace(dest, protect::protection::ReadWriteExecute, size);

    if (protect->invalid()) {
      return {llmo_error::InvalidProtectChangeError};
    }
  }

  // Copy.
  memcpy(dest, src, size);

  // Flush CPU cache.
  flush(dest, size);

  // No errors.
  return std::nullopt;
}

/**
 * @brief Fills a region of memory with a specified value.
 *
 * @details This function fills a contiguous block of memory starting at the
 * given destination address with the specified 8-bit value.  It optionally
 * unprotects the memory region before writing, handling potential errors such
 * as invalid addresses and protection changes. The CPU cache is flushed after
 * the fill operation.
 *
 * @param dest The destination memory address. Must be a valid address.
 * @param value The 8-bit value to fill the memory with.
 * @param size The number of bytes to fill.
 * @param unprotect A boolean flag indicating whether to temporarily unprotect
 * the destination memory region before writing. Defaults to true. Setting this
 * to false can improve performance if the memory is already writable, but
 * requires the caller to ensure the memory is writable.
 * @return An optional llmo_error. std::nullopt indicates success; otherwise,
 * it contains an error code. Possible error codes include:
 *         - llmo_error::InvalidAddressError: The destination address is
 * invalid.
 *         - llmo_error::UnwriteableMemoryError: The memory at the specified
 * address is not writable. This error only occurs if `unprotect` is false.
 *         - llmo_error::InvalidProtectChangeError: An error occurred while
 * attempting to change memory protection. This error only occurs if `unprotect`
 * is true.
 *         - llmo_error::NullSizeError: The size parameter is zero.
 *
 * @note The function uses std::optional for error handling and managing the
 * scoped_protect object, ensuring proper resource management.
 * @note The CPU cache is flushed after the fill operation to ensure the data is
 * visible to other processes and cores.
 *
 * @code{.cpp}
 * // Example usage:
 * char buffer[10];
 * auto error = mywr::llmo::fill(buffer, 0x42, 10);
 * if (!error) {
 *   // Fill successful
 * } else {
 *   // Handle the error: error
 * }
 * @endcode
 */
static auto fill(const address& dest,
                 uint8_t        value,
                 size_t         size,
                 bool unprotect = true) noexcept -> std::optional<llmo_error> {
  using namespace protect;

  if (size == 0) {
    return {llmo_error::NullSizeError};
  }

  if (!dest) {
    return {llmo_error::InvalidAddressError};
  }

  if (!unprotect && !is_writeable(dest)) {
    return {llmo_error::UnwriteableMemoryError};
  }

  std::optional<protect::scoped_protect> protect;

  if (unprotect) {
    protect.emplace(dest, protect::protection::ReadWriteExecute, size);

    if (protect->invalid()) {
      return {llmo_error::InvalidProtectChangeError};
    }
  }

  // Fill the value.
  memset(dest, value, size);

  // Flush the CPU cache.
  flush(dest, size);

  return std::nullopt;
}

/**
 * @brief Compares two memory blocks.
 *
 * @details This function compares two memory blocks of a specified size.  It
 * returns the result of the comparison as an `tl::expected` object.  If either
 * address is invalid, an error is returned. If a memory access error occurs
 * during the comparison, it will return the error indicated.
 *
 * @param buf0 The address of the first memory block. Must be a valid address.
 * @param buf1 The address of the second memory block. Must be a valid address.
 * @param size The number of bytes to compare.
 * @return An `tl::expected<int, llmo_error>` object. The value is the result of
 * memcmp, or an error of type `llmo_error` is returned if an error occurs
 * during the comparison. Possible error codes include:
 *         - llmo_error::InvalidAddressError: Either `buf0` or `buf1` is
 * invalid.
 *         - llmo_error::NullSizeError: The size parameter is zero.
 *
 * @note The function uses `tl::expected` for error handling and managing the
 * memory access, ensuring proper resource management.
 *
 * @code{.cpp}
 * // Example usage:
 * char buf0[10] = "Hello";
 * char buf1[10] = "Hello";
 * auto result = mywr::llmo::compare(buf0, buf1, 10);
 * if (result) {
 *   // Comparison successful: result.value() contains the memcmp result
 * } else {
 *   // Handle the error: result.error()
 * }
 * @endcode
 */
inline auto compare(const address& buf0,
                    const address& buf1,
                    size_t size) noexcept -> tl::expected<int, llmo_error> {
  if (!buf0 || !buf1 || size == 0) {
    return tl::make_unexpected(size == 0 ? llmo_error::NullSizeError
                                         : llmo_error::InvalidAddressError);
  }

  return memcmp(buf0, buf1, size);
}

} // namespace mywr::llmo
