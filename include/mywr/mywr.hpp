/**
 * @file mywr.hpp
 * @author themusaigen
 * @brief All-in-one includer for the mywr library.
 *
 * This header file includes all necessary headers from the mywr library,
 * providing a convenient way to include all functionalities in a single line.
 *
 * @copyright Copyright (c) 2025
 *
 * The library consists of the following modules:
 * - shared/core/os.hpp: Operating system-specific functionality.
 * - shared/core/architecture.hpp: Architecture-specific functionality.
 * - shared/core/core.hpp: Core functionality of the library.
 * - shared/core/version.hpp: Version information of the library.
 * - shared/core/address.hpp: Address manipulation utilities.
 * - shared/protection_flags.hpp: Protection flags and constants.
 * - internal/protect.hpp: Internal implementation details for protection.
 * - internal/llmo.hpp: Low-level memory operations.
 *
 * @note
 * This file is automatically generated and should not be modified directly.
 */
#pragma once

#include <shared/core/os.hpp>
#include <shared/core/architecture.hpp>
#include <shared/core/core.hpp>
#include <shared/core/version.hpp>
#include <shared/core/address.hpp>
#include <shared/protection_flags.hpp>

#include <internal/protect.hpp>
#include <internal/llmo.hpp>
