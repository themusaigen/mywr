/**
 * @file core.hpp
 * @author themusaigen
 * @brief This module contains the main type definitions for the mywr library.
 *
 * The module includes the definition of the byte_t type, which is an alias for
 * std::uint8_t. It also includes the definition of the address_t type, which is
 * an alias for std::uint32_t if the MYWR_32 macro is defined, or std::uint64_t
 * if the MYWR_32 macro is not defined.
 *
 * @copyright Copyright (c) 2025
 */
#pragma once

#if defined(_MSC_VER)
  #pragma warning(disable : 4068) // Disable unknown pragmas warning.
#elif defined(__GNUC__)
  #pragma GCC diagnostic ignored "-Wunknown-pragmas"
  #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
#elif defined(__clang__)
  #pragma clang diagnostic ignored "-Wunknown-pragmas"
  #pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif

// MYWR_32
#include "architecture.hpp"

// uint8_t, uint32_t, uint64
#include <cstdint>

namespace mywr {
using byte_t = std::uint8_t;

#if defined(MYWR_32)
using address_t = std::uint32_t;
#else
using address_t = std::uint64_t;
#endif
} // namespace mywr
