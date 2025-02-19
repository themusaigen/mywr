/**
 * @file abi.hpp
 * @brief Defines a template function to determine the calling convention based
 * on the ABI.
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
