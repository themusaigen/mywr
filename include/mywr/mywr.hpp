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
 * - shared/abi.hpp: Application Binary Interface (ABI) utilities.
 * - shared/calling_conventions.hpp: Calling conventions utilities.
 * - shared/error_handling.hpp: Error handling utilities.
 * - shared/function_traits.hpp: Function traits and type manipulation
 * utilities.
 * - shared/utility.hpp: Various utility functions and classes.
 * - shared/disassembler.hpp: Machine code disassembler implementation using
 * Zydis backend for x86 and x86-64 architectures, providing instruction
 * analysis capabilities.
 * - internal/protect.hpp: Internal implementation details for protection.
 * - internal/llmo.hpp: Low-level memory operations.
 * - internal/module.hpp: Module management utilities.
 * - internal/patch.hpp: Robust and flexible system for applying and managing
 * memory patches in a safe, RAII-compliant manner.
 * - internal/scanner.hpp: Scanner functionality for memory scanning and
 * analysis.
 * - internal/invoker.hpp: Function invocation utilities.
 * - internal/memmap.hpp: Memory mapping utilities.
 * - internal/allocator.hpp: Memory allocation utilities.
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
#include <shared/abi.hpp>
#include <shared/calling_conventions.hpp>
#include <shared/error_handling.hpp>
#include <shared/function_traits.hpp>
#include <shared/utility.hpp>
#include <shared/disassembler.hpp>

#include <internal/protect.hpp>
#include <internal/llmo.hpp>
#include <internal/module.hpp>
#include <internal/patch.hpp>
#include <internal/scanner.hpp>
#include <internal/invoker.hpp>
#include <internal/memmap.hpp>
#include <internal/allocator.hpp>
