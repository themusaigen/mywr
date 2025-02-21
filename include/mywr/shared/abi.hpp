/**
 * @file abi.hpp
 * @brief Defines a template function to determine the calling convention based
 * on the ABI.
 *
 * @details This header file is part of the `mywr` library. It provides a
 * mechanism to determine the appropriate calling convention based on the
 * Application Binary Interface (ABI) of the target platform. The file includes
 * necessary headers for operating system and architecture detection, as well as
 * the calling conventions definitions.
 *
 * The main component of this file is the `calling_convention_by_abi_v` template
 * variable, which is used to select the correct calling convention based on the
 * platform's ABI. The file uses conditional compilation to differentiate
 * between 32-bit and 64-bit Windows platforms, setting the calling convention
 * accordingly.
 *
 * The functionality is encapsulated within the `mywr::detail` namespace,
 * indicating that it is intended for internal use within the library.
 *
 * @note The file currently only provides implementations for Windows platforms,
 * with a distinction between 32-bit and 64-bit architectures. For 64-bit
 * Windows, the `Win64` calling convention is used by default.
 *
 * @see calling_conventions.hpp for the definition of calling conventions.
 * @see os.hpp and architecture.hpp for platform and architecture detection.
 *
 * @author themusaigen
 */
#pragma once

#include <shared/core/os.hpp>
#include <shared/core/architecture.hpp>

#include <shared/calling_conventions.hpp>

namespace mywr::detail {

#if defined(MYWR_WINDOWS)
  #if defined(MYWR_32)
template<calling_conventions C>
inline constexpr auto calling_convention_by_abi_v = C;
  #else
template<calling_conventions C>
inline constexpr auto calling_convention_by_abi_v = calling_conventions::Win64;
  #endif
#endif

} // namespace mywr::detail
