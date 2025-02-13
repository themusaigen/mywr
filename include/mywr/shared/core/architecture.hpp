/**
 * @file architecture.hpp
 * @author themusaigen
 * @brief This module contains macros for identifying the architecture of the
 * system.
 *
 * The macros defined in this module allow for conditional compilation based on
 * the architecture. This is useful for writing architecture-specific code that
 * can be compiled and run on different architectures.
 *
 * The supported architectures are:
 * - MYWR_X86: Defined for 32-bit x86 architecture.
 * - MYWR_X86_64: Defined for 64-bit x86 architecture.
 * - MYWR_ARM: Defined for ARM architecture.
 * - MYWR_AARCH64: Defined for ARM64 architecture.
 *
 * @copyright Copyright (c) 2025
 */
#pragma once

#if defined(_M_IX86) || defined(i386) || defined(__i386) ||                    \
    defined(__i386__) || defined(_X86_)
  #define MYWR_32
  #define MYWR_X86
#elif defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) ||           \
    defined(__x86_64) || defined(__amd64__) || defined(__amd64)
  #define MYWR_64
  #define MYWR_X86_64
#elif defined(__arm__) || defined(__thumb__) || defined(_M_ARM) ||             \
    defined(_M_ARMT)
  #define MYWR_32
  #define MYWR_ARM
#elif defined(__aarch64__)
  #define MYWR_64
  #define MYWR_AARCH64
#endif

#if !defined(MYWR_X86) && !defined(MYWR_X86_64)
  #error "Unsupported architecture.
#endif
