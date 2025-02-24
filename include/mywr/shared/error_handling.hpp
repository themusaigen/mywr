/**
 * @file error_handling.hpp
 * @brief Cross-platform error handling utilities for the mywr library.
 *
 * This file provides error handling functionality that works across different
 * operating systems. It currently supports Windows and has a placeholder for
 * Linux support.
 *
 * @author themusaigen
 */
#pragma once

#include <shared/core/os.hpp>
#include <shared/core/osheaders.hpp>

#include <cstdint>

/**
 * @namespace mywr::error_handling
 * @brief Namespace containing error handling utilities.
 */
namespace mywr::error_handling {
/**
 * @brief Retrieves the last error code from the operating system.
 *
 * This function provides a cross-platform way to get the last error that
 * occurred in the system. On Windows, it wraps the GetLastError() function.
 * For other platforms (e.g., Linux), it currently returns 0 as a placeholder.
 *
 * @return uint32_t The last error code.
 *
 * @note This function is marked as nodiscard to encourage checking its return
 * value.
 * @note On non-Windows platforms, this function currently returns 0. Future
 *       implementations should return errno for Linux and other POSIX-compliant
 * systems.
 *
 * @todo Implement proper error code retrieval for Linux using errno.
 */
[[nodiscard]] inline auto get_last_error() noexcept -> uint32_t {
#if defined(MYWR_WINDOWS)
  return GetLastError();
#else
  // TODO: (linux) return errno
  return 0;
#endif
}
} // namespace mywr::error_handling
