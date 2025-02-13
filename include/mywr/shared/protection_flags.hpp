/**
 * @file protection_flags.hpp
 * @author themusaigen
 * @brief Module that contains protection flags constants for memory and code
 * sections.
 *
 * @copyright Copyright (c) 2025
 *
 * This module provides an enumeration of protection flags used in memory and
 * code sections. It also includes functions to convert between system-specific
 * protection constants and the `mywr` protection constant.
 *
 * The `protection` enum class contains the following flags:
 * - None: No access to the memory or code section.
 * - NoAccess: No access to the memory or code section.
 * - Read: Read access to the memory or code section.
 * - Write: Write access to the memory or code section.
 * - Execute: Execute access to the memory or code section.
 * - ReadWrite: Combination of Read and Write access.
 * - ReadExecute: Combination of Read and Execute access.
 * - ReadWriteExecute: Combination of Read, Write, and Execute access.
 *
 * The `to_protection_constant` function converts a system-specific protection
 * constant to a `mywr` protection constant.
 *
 * The `from_protection_constant` function converts a `mywr` protection constant
 * to a system-specific protection constant.
 *
 * The module includes platform-specific implementations for the conversion
 * functions. For Windows, the implementation is located in the
 * `shared/windows/protection_flags.inl` file.
 */
#pragma once

#include <shared/core/os.hpp>

#include <cstdint>

/**
 * @brief Namespace that contating protection utilities.
 */
namespace mywr::protect {
enum class protection : uint8_t {
  None     = (1 << 0),
  NoAccess = (1 << 1),
  Read     = (1 << 2),
  Write    = (1 << 3),
  Execute  = (1 << 4),

  ReadWrite        = (Read | Write),
  ReadExecute      = (Read | Execute),
  ReadWriteExecute = (Read | Write | Execute)
};

inline auto operator&(const protection& lhs, const protection& rhs) noexcept
    -> uint8_t {
  return static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs);
}

/**
 * @brief Transforms system protection constant to `mywr` constant.
 *
 * @param protect System-specific protection constant.
 * @return Corresponding `mywr` protection constant.
 *
 * This function converts a system-specific protection constant to a `mywr`
 * protection constant. The conversion is platform-specific and is implemented
 * in the corresponding platform-specific file (e.g.,
 * `shared/windows/protection_flags.inl` for Windows).
 */
static auto to_protection_constant(uint32_t protect) -> protection;

/**
 * @brief Transforms `mywr` protection constant to system protection constant.
 *
 * @param protect `mywr` protection constant.
 * @return Corresponding system-specific protection constant.
 *
 * This function converts a `mywr` protection constant to a system-specific
 * protection constant. The conversion is platform-specific and is implemented
 * in the corresponding platform-specific file (e.g.,
 * `shared/windows/protection_flags.inl` for Windows).
 */
static auto from_protection_constant(protection protect) -> uint32_t;

} // namespace mywr::protect

#if defined(MYWR_WINDOWS)
  #include <shared/windows/protection_flags.inl>
#endif
