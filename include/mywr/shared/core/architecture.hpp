/**
 * @file architecture.hpp
 * @author themusaigen
 * @brief Module that contains macros of the architecture.
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MYWR_SHARED_ARCHITECTURE_HPP_
#define MYWR_SHARED_ARCHITECTURE_HPP_

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

#endif
