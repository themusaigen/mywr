/**
 * @file osheaders.hpp
 * @author themusaigen
 * @brief This meta-file includes system headers based on the detected operating
 * system.
 *
 * The osheaders.hpp file includes the appropriate system headers based on the
 * operating system defined in the os.hpp file. This allows for
 * platform-specific code to be written that can be compiled and run on
 * different operating systems.
 *
 * The supported operating systems and their corresponding system headers are:
 * - MYWR_WINDOWS: Includes <Windows.h> for Windows operating systems.
 *
 * @copyright Copyright (c) 2025
 */
#pragma once

#include "os.hpp"

#if defined(MYWR_WINDOWS)
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif

  #include <Windows.h>
#endif
