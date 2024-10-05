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
  #if defined(MYWR_WINDOWS)
    #define MYWR_STDCALL __attribute__((stdcall))
    #define MYWR_THISCALL __attribute__((thiscall))
    #define MYWR_FASTCALL __attribute__((fastcall))
  #else
    #define MYWR_STDCALL MYWR_CDECL
    #define MYWR_THISCALL MYWR_CDECL
    #define MYWR_FASTCALL MYWR_CDECL
  #endif
#else
  #define MYWR_CDECL __cdecl
  #if defined(MYWR_WINDOWS)
    #define MYWR_STDCALL __stdcall
    #define MYWR_THISCALL __thiscall
    #define MYWR_FASTCALL __fastcall
  #else
    #define MYWR_STDCALL MYWR_CDECL
    #define MYWR_THISCALL MYWR_CDECL
    #define MYWR_FASTCALL MYWR_CDECL
  #endif
#endif

namespace mywr {
namespace detail {
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

constexpr auto abi_v = system_abi::abi;

constexpr auto is_win32_abi = abi_v == system_abi::kWin32;
constexpr auto is_unix_abi = abi_v == system_abi::kUnix;
constexpr auto is_win64_abi = abi_v == system_abi::kWin64;
constexpr auto is_systemv_abi = abi_v == system_abi::kSystemV;

template <calling_conventions::Enum, system_abi::Enum>
struct calling_convention_by_abi;

template <calling_conventions::Enum Convention>
struct calling_convention_by_abi<Convention, system_abi::kWin32> {
  static constexpr auto value = Convention;
};

template <calling_conventions::Enum Convention>
struct calling_convention_by_abi<Convention, system_abi::kUnix> {
  static constexpr auto value = calling_conventions::kCdecl;
};

template <calling_conventions::Enum Convention>
struct calling_convention_by_abi<Convention, system_abi::kWin64> {
  static constexpr auto value = calling_conventions::kWin64;
};

template <calling_conventions::Enum Convention>
struct calling_convention_by_abi<Convention, system_abi::kSystemV> {
  static constexpr auto value = calling_conventions::kSystemV;
};

template <calling_conventions::Enum Convention>
constexpr auto calling_convention_by_abi_v =
    calling_convention_by_abi<Convention, system_abi::abi>::value;

template <typename T>
struct is_non_pod;

template <>
struct is_non_pod<void> {
  static constexpr auto value = 0;
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

// Old GCC versions have a bug with stack aligned to a 16-byte boundary as
// mentioned at https://en.wikipedia.org/wiki/X86_calling_conventions. We can't
// know for sure if this bug is present or not, as it depends on the
// compiler -mpreffered-stack-boundary parameter, as I understand it.
constexpr auto kStackAlignment = sizeof(mywr::address_t);

template <typename T>
constexpr auto align_parameter_size() {
  if constexpr (std::is_reference_v<T> || std::is_pointer_v<T>)
    return kStackAlignment;

  constexpr auto remainder = sizeof(T) % kStackAlignment;
  return remainder == 0 ? sizeof(T) : sizeof(T) + kStackAlignment - remainder;
}

template <typename T, typename... Ts>
constexpr auto align_parameters_size() {
  if constexpr (sizeof...(Ts) > 0)
    return align_parameter_size<T>() + align_parameters_size<Ts...>();
  else
    return align_parameter_size<T>();
}

template <typename T, typename... Ts>
constexpr auto sizeof_parameters_size() {
  if constexpr (sizeof...(Ts) > 0)
    return sizeof(T) + sizeof_parameters_size<Ts...>();
  else
    return sizeof(T);
}

template <typename T>
struct sizeof_tuple;

template <>
struct sizeof_tuple<std::tuple<>> {
  static constexpr auto value = 0;
};

template <typename... Args>
struct sizeof_tuple<std::tuple<Args...>> {
  static constexpr auto value = sizeof_parameters_size<Args...>();
};

template <typename Tuple>
constexpr auto sizeof_tuple_v = sizeof_tuple<Tuple>::value;

template <typename T>
struct align_tuple_params_size;

template <>
struct align_tuple_params_size<std::tuple<>> {
  static constexpr auto value = 0;
};

template <typename... Args>
struct align_tuple_params_size<std::tuple<Args...>> {
  static constexpr auto value = align_parameters_size<Args...>();
};

template <typename Tuple>
constexpr auto align_tuple_params_size_v =
    align_tuple_params_size<Tuple>::value;
} // namespace detail
} // namespace mywr

namespace mywr {
/**
 * Namespace containing function traits.
 */
namespace traits {
template <typename Ret, typename... Args>
struct function_trait {
  using return_type = Ret;
  using arguments = std::tuple<Args...>;

  static constexpr auto args_count = sizeof...(Args);
  static constexpr auto args_size = detail::sizeof_tuple_v<std::tuple<Args...>>;
  static constexpr auto aligned_args_size =
      detail::align_tuple_params_size_v<std::tuple<Args...>>;

  static constexpr auto is_return_non_pod = detail::is_non_pod_v<Ret>;
};

template <typename Fun>
struct function_traits;

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

#if defined(MYWR_WINDOWS)
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

template <typename Ret, typename Class, typename... Args>
struct function_traits<Ret (Class::*)(Args...)>
    : function_trait<Ret, Class*, Args...> {
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

template <typename Fun>
using return_type_t = typename function_traits<Fun>::return_type;

template <typename Fun>
using arguments_t = typename function_traits<Fun>::arguments;

template <typename Fun>
constexpr auto args_count_v = function_traits<Fun>::args_count;

template <typename Fun>
constexpr auto args_size_v = function_traits<Fun>::args_size;

template <typename Fun>
constexpr auto aligned_args_size_to_stack_v =
    function_traits<Fun>::aligned_args_size;

template <typename Fun>
constexpr auto is_return_non_pod_v = function_traits<Fun>::is_return_non_pod;

template <typename Fun>
constexpr auto convention_v = function_traits<Fun>::convention;

} // namespace traits
} // namespace mywr

#endif // !MYWR_TRAITS_HPP_
