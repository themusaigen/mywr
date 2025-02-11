/**
 * @file os.hpp
 * @author themusaigen
 * @brief Module that contains macros of the current operating system.
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
