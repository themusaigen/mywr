/*********************************************************************
 * @file   mywr.hpp
 * @brief  The core file of `memwrapper` library.
 *
 * @author themusaigen
 * @date   September 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_HPP_
#define MYWR_HPP_

constexpr auto MYWR_MAJOR = 1;
constexpr auto MYWR_MINOR = 2;
constexpr auto MYWR_PATCH = 0;
constexpr auto MYWR_VERSION =
    MYWR_MAJOR * 10000 + MYWR_MINOR * 100 + MYWR_PATCH * 10;
constexpr auto MYWR_VERSION_STR = "1.2.0";

#if defined(__GNUC__)
  #define MYWR_GCC
#elif defined(_MSC_VER)
  #define MYWR_MSVC
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__ppc64__) ||            \
    defined(__MINGW64__)
  #define MYWR_X64
#elif defined(i386) || defined(__i386__) || defined(__i386) ||                 \
    defined(_M_IX86) || defined(_X86_)
  #define MYWR_X86
#else
  #error "Unknown architecture."
#endif

#if defined(_WIN64) || defined(_WIN32) || defined(__CYGWIN__)
  #define MYWR_WINDOWS
#elif defined(__linux__)
  #define MYWR_LINUX
  #define MYWR_UNIX
#elif defined(__unix__)
  #define MYWR_UNIX
#else
  #error "Unknown OS."
#endif

#if defined(_MSVC_LANG)
  #define MYWR_CXX _MSVC_LANG
#else
  #define MYWR_CXX __cplusplus
#endif

#if defined(MYWR_MSVC)
  #define MYWR_INLINE inline
  #define MYWR_FORCEINLINE __forceinline
#elif defined(MYWR_GCC)
  #define MYWR_INLINE inline __attribute__((always_inline))
  #define MYWR_FORCEINLINE MYWR_INLINE
#else
  #define MYWR_INLINE inline
  #define MYWR_FORCEINLINE MYWR_INLINE
#endif

#if defined(__has_include)
  #define MYWR_HAS_INCLUDE __has_include
#else
  #define MYWR_HAS_INCLUDE 0
#endif

#if !(MYWR_CXX >= 202002L)
  #error "only c++20 and newer."
#endif

#if defined(MYWR_WINDOWS)
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif

  #include <Windows.h>
#elif defined(MYWR_UNIX)
  #include <filesystem>
  #include <fstream>
  #include <charconv>

  // clang-format off
  #if MYWR_HAS_INCLUDE(<sys/cachectl.h>)
    #include <sys/cachectl.h>
  #elif MYWR_HAS_INCLUDE(<asm/cachectl.h>)
    #include <asm/cachectl.h>
  #else
    #define MYWR_FEATURE_NO_FLUSH_CACHE
  #endif

  #if MYWR_HAS_INCLUDE(<sys/mman.h>)
    #include <sys/mman.h>
  #elif MYWR_HAS_INCLUDE(<asm/mman.h>)
    #include <asm/mman.h>
  #else
    #define MYWR_FEATURE_NO_MPROTECT
  #endif
  // clang-format on
#endif

/**
 * @brief The core namespace of the `memwrapper` library.
 */
namespace mywr {
/**
 * @brief An alias for `uint8_t`.
 */
using byte_t = std::uint8_t;
/**
 * @brief An alias for `uint32_t` or `uint64_t` based on current
 * architecture.
 */
#if defined(MYWR_X86)
using address_t = std::uint32_t;
#else
using address_t = std::uint64_t;
#endif
} // namespace mywr

/// Cxx Libraries.
#include <type_traits>
#include <map>
#include <tuple>

/// Internal Libraries.
#include "x86_64/address.hpp"
#include "x86_64/detail.hpp"
#include "x86_64/traits.hpp"
#include "x86_64/protect.hpp"
#include "x86_64/llmo.hpp"
#include "x86_64/invoker.hpp"

#endif // !MYWR_HPP_
