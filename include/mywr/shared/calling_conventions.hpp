/**
 * @file calling_conventions.hpp
 * @brief Defines an enumeration for different calling conventions used in C++.
 *
 * @details This header file is part of the `mywr` library. It defines an
 * enumeration for various calling conventions used in C++ programming. The file
 * also includes preprocessor directives to define calling conventions for
 * different compilers, ensuring compatibility across platforms. The supported
 * calling conventions are:
 * - Cdecl: The C declaration calling convention, which is the default for C and
 * C++ programs.
 * - Stdcall: The standard calling convention used in Windows API functions,
 * where the callee cleans the stack.
 * - Thiscall: The calling convention used for member functions in C++, where
 * the `this` pointer is passed in a register.
 * - Fastcall: The fast calling convention used for functions with many integer
 * arguments, where arguments are passed in registers.
 * - Win64: The calling convention used for 64-bit Windows functions, which is a
 * variation of the fastcall convention.
 *
 * The file uses conditional compilation to define the appropriate calling
 * convention macros based on the compiler being used. For GCC, it uses
 * `__attribute__` syntax, while for other compilers, it uses the standard
 * calling convention keywords.
 *
 * The calling conventions are encapsulated within the `mywr::detail` namespace,
 * indicating that they are intended for internal use within the library.
 *
 * @note The file ensures that the calling conventions are defined consistently
 * across different compilers, providing a unified interface for function
 * declarations in the library.
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
/**
 * @brief Enumeration of calling conventions.
 *
 * @details This enumeration defines the different calling conventions supported
 * by the library. It is used to specify the calling convention of a function
 * when extracting or manipulating function type information.
 */
enum class calling_conventions : std::uint8_t {
  Cdecl,    ///< C declaration calling convention.
  Stdcall,  ///< Standard calling convention used in Windows API functions.
  Thiscall, ///< Calling convention used for member functions in C++.
  Fastcall, ///< Fast calling convention used for functions with many integer
            ///< arguments.
  Win64     ///< Calling convention used for 64-bit Windows functions.
};
} // namespace mywr::detail
