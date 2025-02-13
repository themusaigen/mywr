/**
 * @file os.hpp
 * @author themusaigen
 * @brief This module contains macros for identifying the current operating
 * system.
 *
 * The macros defined in this module allow for conditional compilation based on
 * the operating system. This is useful for writing platform-independent code
 * that can be compiled and run on different operating systems.
 *
 * @copyright Copyright (c) 2025
 */
#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
  #define MYWR_WINDOWS
#elif defined(__unix__)
  #define MYWR_UNIX
#elif defined(__APPLE__)
  #define MYWR_MACOS
#elif defined(__ANDROID__)
  #define MYWR_ANDROID
#endif

#if !defined(MYWR_WINDOWS)
  #error "Unsupported OS."
#endif
