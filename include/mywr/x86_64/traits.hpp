/*********************************************************************
 * @file   traits.hpp
 * @brief  Module containing function traits.
 *
 * @author themusaigen
 * @date   October 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_TRAITS_HPP_
#define MYWR_TRAITS_HPP_

#if defined(MYWR_GCC)
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

namespace mywr {
namespace detail {
/**
 * @class calling_conventions
 * @brief Contains all supported calling conventions on 86 and 64-bit
 * architecture.
 */

/**
 * @enum calling_conventions::Enum
 * @brief Contains all supported calling conventions on 86 and 64-bit
 * architecture.
 */

/**
 * @var calling_conventions::Enum calling_conventions::kUnknown
 * @brief Unsupported calling convention.
 */

/**
 * @var calling_conventions::Enum calling_conventions::kCdecl
 * @brief C declaration calling-conventions. Stack uses RTL order. Cleanup by
 * caller.
 */

/**
 * @var calling_conventions::Enum calling_conventions::kStdcall
 * @brief Standard calling convention for Microsoft Win32 API. Stack uses RTL
 * order. Cleanup by callee.
 */

/**
 * @var calling_conventions::Enum calling_conventions::kThiscall
 * @brief Used for calling C++ non-static member functions. Stack uses RTL
 * order. Cleanup by callee.
 *
 * @details
 * For the GCC compiler, thiscall is almost identical to cdecl: The caller
 * cleans the stack, and the parameters are passed in right-to-left order. The
 * difference is the addition of the this pointer, which is pushed onto the
 * stack last, as if it were the first parameter in the function
 * prototype.
 *
 * On the Microsoft Visual C++ compiler, the this pointer is passed in ECX and
 * it is the callee that cleans the stack, mirroring the stdcall convention used
 * in C for this compiler and in Windows API functions. When functions use a
 * variable number of arguments, it is the caller that cleans the stack (cf.
 * cdecl). See
 * https://en.wikipedia.org/wiki/X86_calling_conventions#Callee_clean-up
 */

/**
 * @var calling_conventions::Enum calling_conventions::kFastcall
 * @brief Microsoft __fastcall convention passes the first
 * two arguments (evaluated left to right) that fit, into ECX and EDX.
 */

/**
 * @var calling_conventions::Enum calling_conventions::kWin64
 * @brief Convention that used for all functions (except __vectorcall) Windows
 * 64-bit architecture.
 */

/**
 * @var calling_conventions::Enum calling_conventions::kSystemV
 * @brief Convention that used for all functions (except __vectorcall) Unix-like
 * systems 64-bit architecture.
 */

class calling_conventions {
public:
  enum Enum : std::uint32_t {
    kUnknown,
    kCdecl,
    kStdcall,
    kThiscall,
    kFastcall,
    kWin64,
    kSystemV
  };
};

/**
 * @class system_abi
 * @brief Contains all supported ABI.
 */

/**
 * @enum system_abi::Enum
 * @brief Contains all supported ABI.
 */

/**
 * @var system_abi::Enum system_abi::kUnknown
 * @brief Unsupported ABI.
 */

/**
 * @var system_abi::Enum system_abi::kWin32
 * @brief Windows 32-bit ABI with cdecl, stdcall, thiscall, etc... calling
 * conventions.
 */

/**
 * @var system_abi::Enum system_abi::kUnix
 * @brief Unix-like 32-bit systems ABI.
 */

/**
 * @var system_abi::Enum system_abi::kWin64
 * @brief Windows 64-bit ABI with only one main calling convention similar to
 * fastcall.
 */

/**
 * @var system_abi::Enum system_abi::kSystemV
 * @brief Unix-like 64-bit systems ABI with only one main calling convention
 * similar to fastcall.
 */

/**
 * @var system_abi::abi
 * @brief Contains current ABI based on preprocessor defines.
 */
class system_abi {
public:
  enum Enum : std::uint32_t { kUnknown, kWin32, kUnix, kWin64, kSystemV };

#if defined(MYWR_X86)
  #if defined(MYWR_WINDOWS)
  static constexpr auto abi = kWin32;
  #elif defined(MYWR_UNIX)
  static constexpr auto abi = kUnix;
  #endif
#elif defined(MYWR_X64)
  #if defined(MYWR_WINDOWS)
  static constexpr auto abi = kWin64;
  #elif defined(MYWR_UNIX)
  static constexpr auto abi = kSystemV;
  #endif
#else
  static constexpr auto abi = kUnknown;
#endif
};

/**
 * @brief Contains current ABI based on preprocessor defines.
 */
constexpr auto abi_v = system_abi::abi;

/**
 * @brief Compile-time flag is current ABI is Win32.
 */
constexpr auto is_win32_abi = abi_v == system_abi::kWin32;

/**
 * @brief Compile-time flag is current ABI is Unix-like 32-bit.
 */
constexpr auto is_unix_abi = abi_v == system_abi::kUnix;

/**
 * @brief Compile-time flag is current ABI is Win64.
 */
constexpr auto is_win64_abi = abi_v == system_abi::kWin64;

/**
 * @brief Compile-time flag is current ABI is SystemV.
 */
constexpr auto is_systemv_abi = abi_v == system_abi::kSystemV;

// This trick is used to avoid doxygen documentation.
#if (true)
template <calling_conventions::Enum, system_abi::Enum>
struct calling_convention_by_abi;

template <calling_conventions::Enum Convention>
struct calling_convention_by_abi<Convention, system_abi::kWin32> {
  static constexpr auto value = Convention;
};

template <calling_conventions::Enum Convention>
struct calling_convention_by_abi<Convention, system_abi::kUnix> {
  static constexpr auto value = Convention;
};

template <calling_conventions::Enum Convention>
struct calling_convention_by_abi<Convention, system_abi::kWin64> {
  static constexpr auto value = calling_conventions::kWin64;
};

template <calling_conventions::Enum Convention>
struct calling_convention_by_abi<Convention, system_abi::kSystemV> {
  static constexpr auto value = calling_conventions::kSystemV;
};
#endif

/**
 * @brief Converts the calling convention according to the current system ABI.
 *
 * @details
 * Each architecture or system has its own unique calling convention.
 * On Windows 32 bit, there are basic conventions: cdecl, stdcall, thiscall,
 * fastcall, and so on.
 * On Unix-like 32 bit, there is only one calling convention: cdecl.
 * On Windows and Unix-like systems with 64-bit architecture, there is only one
 * calling convention used by all functions.
 */
template <calling_conventions::Enum Convention>
constexpr auto calling_convention_by_abi_v =
    calling_convention_by_abi<Convention, system_abi::abi>::value;

/**
 * @brief Indicates whether the specified type is not a POD (Plain old data)
 * type
 */
template <typename T>
struct is_non_pod;

#if (true)
template <>
struct is_non_pod<void> {
  static constexpr auto value = false;
};

template <typename T>
struct is_non_pod {
  #if defined(MYWR_WINDOWS)
  static constexpr auto value = sizeof(T) > 8 || !std::is_trivial_v<T> ||
                                !std::is_trivially_destructible_v<T>;
  #else
  static constexpr auto value = sizeof(T) > 8 || !std::is_trivial_v<T> ||
                                !std::is_trivially_destructible_v<T> ||
                                std::is_class_v<T> || std::is_union_v<T>;
  #endif
};

template <typename T>
constexpr auto is_non_pod_v = is_non_pod<T>::value;
#endif

// Old GCC versions have a bug with stack aligned to a 16-byte boundary as
// mentioned at https://en.wikipedia.org/wiki/X86_calling_conventions. We can't
// know for sure if this bug is present or not, as it depends on the
// compiler -mpreffered-stack-boundary parameter, as I understand it.

/**
 * @brief The alignment of the size of each type on the stack.
 */
constexpr auto kStackAlignment = sizeof(mywr::address_t);

/**
 * @brief Returns the size of the type by aligning it to match the stack
 * alignment.
 */
template <typename T>
constexpr auto align_parameter_size() {
  if constexpr (std::is_reference_v<T> || std::is_pointer_v<T>)
    return kStackAlignment;

  constexpr auto remainder = sizeof(T) % kStackAlignment;
  return remainder == 0 ? sizeof(T) : sizeof(T) + kStackAlignment - remainder;
}

/**
 * @brief Returns the size of the types by aligning them to match the stack
 * alignment.
 */
template <typename T, typename... Ts>
constexpr auto align_parameters_size() {
  if constexpr (sizeof...(Ts) > 0)
    return align_parameter_size<T>() + align_parameters_size<Ts...>();
  else
    return align_parameter_size<T>();
}

/**
 * @brief Returns the size of the passed parameters.
 */
template <typename T, typename... Ts>
constexpr auto sizeof_parameters_size() {
  if constexpr (sizeof...(Ts) > 0)
    return sizeof(T) + sizeof_parameters_size<Ts...>();
  else
    return sizeof(T);
}

#if (true)
template <typename T>
struct sizeof_tuple_params;

template <>
struct sizeof_tuple_params<std::tuple<>> {
  static constexpr auto value = 0;
};

template <typename... Args>
struct sizeof_tuple_params<std::tuple<Args...>> {
  static constexpr auto value = sizeof_parameters_size<Args...>();
};
#endif

/**
 * @brief Returns the size of the parameter types in the tuple.
 */
template <typename Tuple>
constexpr auto sizeof_tuple_params_v = sizeof_tuple_params<Tuple>::value;

#if (true)
template <typename>
struct align_tuple_params_size;

template <>
struct align_tuple_params_size<std::tuple<>> {
  static constexpr auto value = 0;
};

template <typename... Args>
struct align_tuple_params_size<std::tuple<Args...>> {
  static constexpr auto value = align_parameters_size<Args...>();
};
#endif

/**
 * @brief Returns the size of the parameter types in the tuple aligned them to
 * match stack alignment.
 */
template <typename Tuple>
constexpr auto align_tuple_params_size_v =
    align_tuple_params_size<Tuple>::value;
} // namespace detail
} // namespace mywr

namespace mywr {
/**
 * @brief Namespace containing function traits.
 */
namespace traits {

/**
 * @brief Characterizes a function based on the type of return value and
 * arguments.
 */
template <typename Ret, typename... Args>
struct function_trait {
  /**
   * @brief The type of return value.
   */
  using return_type = Ret;

  /**
   * @brief The tuple of passed function parameters.
   */
  using arguments = std::tuple<Args...>;

  /**
   * @brief The arguments count.
   */
  static constexpr auto args_count = sizeof...(Args);

  /**
   * @brief The size of all arguments in the function.
   */
  static constexpr auto args_size =
      detail::sizeof_tuple_params_v<std::tuple<Args...>>;

  /**
   * @brief The aligned to stack alignment size of all arguments in the
   * function.
   */
  static constexpr auto aligned_args_size =
      detail::align_tuple_params_size_v<std::tuple<Args...>>;

  /**
   * @brief Is type of return value is non POD (Plain old data).
   */
  static constexpr auto is_return_non_pod = detail::is_non_pod_v<Ret>;
};

/**
 * @brief Gives a more detailed description of the function by adding a calling
 * convention according to the current ABI.
 */
template <typename Fun>
struct function_traits;

#if (true)
template <typename Ret, typename... Args>
struct function_traits<Ret(MYWR_CDECL*)(Args...)>
    : function_trait<Ret, Args...> {
  static constexpr auto convention =
      detail::calling_convention_by_abi_v<detail::calling_conventions::kCdecl>;
};

template <typename Ret, typename... Args>
struct function_traits<Ret MYWR_CDECL(Args...)> : function_trait<Ret, Args...> {
  static constexpr auto convention =
      detail::calling_convention_by_abi_v<detail::calling_conventions::kCdecl>;
};

template <typename Ret, typename Class, typename... Args>
struct function_traits<Ret (Class::*)(Args...)>
    : function_trait<Ret, Class*, Args...> {
  #if defined(MYWR_WINDOWS)
  static constexpr auto convention = detail::calling_convention_by_abi_v<
      detail::calling_conventions::kThiscall>;
  #else
  static constexpr auto convention =
      detail::calling_convention_by_abi_v<detail::calling_conventions::kCdecl>;
  #endif
};

  #if defined(MYWR_X86)
template <typename Ret, typename... Args>
struct function_traits<Ret(MYWR_STDCALL*)(Args...)>
    : function_trait<Ret, Args...> {
  static constexpr auto convention = detail::calling_convention_by_abi_v<
      detail::calling_conventions::kStdcall>;
};

template <typename Ret, typename... Args>
struct function_traits<Ret MYWR_STDCALL(Args...)>
    : function_trait<Ret, Args...> {
  static constexpr auto convention = detail::calling_convention_by_abi_v<
      detail::calling_conventions::kStdcall>;
};

template <typename Ret, typename... Args>
struct function_traits<Ret(MYWR_THISCALL*)(Args...)>
    : function_trait<Ret, Args...> {
  static constexpr auto convention = detail::calling_convention_by_abi_v<
      detail::calling_conventions::kThiscall>;
};

template <typename Ret, typename... Args>
struct function_traits<Ret(MYWR_FASTCALL*)(Args...)>
    : function_trait<Ret, Args...> {
  static constexpr auto convention = detail::calling_convention_by_abi_v<
      detail::calling_conventions::kFastcall>;
};
  #endif
#endif

/**
 * @brief A shortcut to get the function`s return type.
 */
template <typename Fun>
using return_type_t = typename function_traits<Fun>::return_type;

/**
 * @brief A shortcut to get the function`s arguments tuple.
 */
template <typename Fun>
using arguments_t = typename function_traits<Fun>::arguments;

/**
 * @brief A shortcut to get the function`s arguments count.
 */
template <typename Fun>
constexpr auto args_count_v = function_traits<Fun>::args_count;

/**
 * @brief A shortcut to get the functions`s arguments size.
 */
template <typename Fun>
constexpr auto args_size_v = function_traits<Fun>::args_size;

/**
 * @brief A shortcut to get the functions`s arguments size aligned to match
 * stack alignment.
 */
template <typename Fun>
constexpr auto aligned_args_size_to_stack_v =
    function_traits<Fun>::aligned_args_size;

/**
 * @brief A shortcut to identify is function returnin non-pod type.
 */
template <typename Fun>
constexpr auto is_return_non_pod_v = function_traits<Fun>::is_return_non_pod;

/**
 * @brief A shortcut to get the function`s calling convention.
 */
template <typename Fun>
constexpr auto convention_v = function_traits<Fun>::convention;

/**
 * @brief A shortcut to identify is function a cdecl.
 */
template <typename Fun>
constexpr auto is_cdecl_v =
    convention_v<Fun> == detail::calling_conventions::kCdecl;

/**
 * @brief A shortcut to identify is function a stdcall.
 */
template <typename Fun>
constexpr auto is_stdcall_v =
    convention_v<Fun> == detail::calling_conventions::kStdcall;

/**
 * @brief A shortcut to identify is function a thiscall.
 */
template <typename Fun>
constexpr auto is_thiscall_v =
    convention_v<Fun> == detail::calling_conventions::kThiscall;

/**
 * @brief A shortcut to identify is function a fastcall.
 */
template <typename Fun>
constexpr auto is_fastcall_v =
    convention_v<Fun> == detail::calling_conventions::kFastcall;

/**
 * @brief A shortcut to identify is function have Win64 calling convention.
 */
template <typename Fun>
constexpr auto is_win64_v =
    convention_v<Fun> == detail::calling_conventions::kWin64;

/**
 * @brief A shortcut to identify is function have SystemV calling convention.
 */
template <typename Fun>
constexpr auto is_systemv_v =
    convention_v<Fun> == detail::calling_conventions::kSystemV;

} // namespace traits
} // namespace mywr

#endif // !MYWR_TRAITS_HPP_
