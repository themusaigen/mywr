/**
 * @file calling_conventions.hpp
 * @brief Defines an enumeration for different calling conventions used in C++.
 *
 * @details The file also includes preprocessor directives to define calling
 * conventions for different compilers. The supported calling conventions are:
 *          - Cdecl: The C declaration calling convention.
 *          - Stdcall: The standard calling convention used in Windows API
 * functions.
 *          - Thiscall: The calling convention used for member functions in C++.
 *          - Fastcall: The fast calling convention used for functions with many
 * integer arguments.
 *          - Win64: The calling convention used for 64-bit Windows functions.
 *
 * @author themusaigen
 */
#pragma once

#include <cstdint>

#if defined(__GNUC__)
  #define MYWR_CDECL __attribute__((cdecl))
  #define MYWR_STDCALL __attribute__((stdcall))
  #define MYWR_THISCALL __attribute__((thiscall))
  #define MYWR_FASTCALL __attribute__((fastcall))
#else
  #define MYWR_CDECL __cdecl
  #define MYWR_STDCALL __stdcall
  #define MYWR_THISCALL __thiscall
  #define MYWR_FASTCALL __fastcall
#endif

namespace mywr::detail {
enum class calling_conventions : std::uint8_t {
  Cdecl,
  Stdcall,
  Thiscall,
  Fastcall,
  Win64
};
} // namespace mywr::detail
