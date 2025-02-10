/**
 * @file core.hpp
 * @author themusaigen
 * @brief Module that contains the main type definitions.
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MYWR_SHARED_CORE_HPP_
#define MYWR_SHARED_CORE_HPP_

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

#endif
