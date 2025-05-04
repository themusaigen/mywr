#pragma once

#if defined(_MSC_VER)
  #define MYWR_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
  #define MYWR_NOINLINE __attribute__((noinline))
#elif defined(__clang__)
  #define MYWR_NOINLINE __attribute__((noinline))
#elif defined(__MINGW32__) || defined(__MINGW64__)
  #define MYWR_NOINLINE __attribute__((noinline))
#else
  #define MYWR_NOINLINE
#endif
