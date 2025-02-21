/**
 * @file function_traits.hpp
 * @brief Provides template meta-programming utilities for extracting and
 * manipulating function type information.
 *
 * @details This header file is part of the `mywr` library. It defines a set of
 * template structures and type aliases that facilitate the extraction and
 * manipulation of function type information using template meta-programming
 * techniques. The file is designed to be part of a header-only library, as
 * indicated by the use of `#pragma once` to prevent multiple inclusions.
 *
 * The functionality is encapsulated within the `mywr::detail` namespace, which
 * suggests that it is intended for internal use within the library. The file
 * includes headers from the `shared` directory, which likely provide ABI
 * definitions and calling convention utilities.
 *
 * The main components of this file are the `function_trait` and
 * `function_traits` template structures, which extract return types, argument
 * types, and calling conventions from function types. Additionally, the file
 * provides meta-functions for concatenating calling conventions with function
 * signatures.
 *
 * The file also defines several type aliases and constexpr variables for
 * simplified usage of the extracted function type information.
 *
 * @note The file includes specializations for different calling conventions,
 * such as Cdecl, Stdcall, Thiscall, and Fastcall, as well as for
 * `std::function` and member function pointers.
 *
 * @author themusaigen
 */
#pragma once

#include <shared/abi.hpp>
#include <shared/calling_conventions.hpp>

#include <cstddef>
#include <tuple>
#include <functional>

namespace mywr::detail {

/**
 * @brief Primary template for extracting function type information.
 *
 * @tparam F The function type.
 */
template<typename F>
struct function_trait;

/**
 * @brief Primary template for extracting function type information with
 * additional specializations.
 *
 * @tparam F The function type.
 */
template<typename F>
struct function_traits;

// Implementation of function_trait.
template<typename R, typename... A>
struct function_trait<R(A...)> {
  using return_type = R;                ///< The return type of the function.
  using arguments   = std::tuple<A...>; ///< A tuple of argument types.

  /**
   * @brief Extracts the type of the I-th argument.
   *
   * @tparam I The index of the argument.
   */
  template<std::size_t I>
  using argument = std::tuple_element<I, std::tuple<A...>>;

  static constexpr auto args_count = sizeof...(A); ///< The number of arguments.
};

// Additional specializations for std::function and different calling
// conventions.

template<typename R, typename... A>
struct function_traits<R(A...)> : function_trait<R(A...)> {
  static constexpr auto convention =
      calling_convention_by_abi_v<calling_conventions::Cdecl>;
};

template<typename R, typename... A>
struct function_traits<std::function<R(A...)>> : function_traits<R(A...)> {};

#if defined(MYWR_64)
template<typename R, typename... A>
struct function_traits<R (*)(A...)> : function_traits<R(A...)> {};
#endif

// On 64 bit arhitectures, the compiler will be ignore MYWR_*CALL.
#if defined(MYWR_32)
template<typename R, typename... A>
struct function_traits<R(MYWR_CDECL*)(A...)> : function_trait<R(A...)> {
  static constexpr auto convention =
      calling_convention_by_abi_v<calling_conventions::Cdecl>;
};

template<typename R, typename... A>
struct function_traits<R(MYWR_STDCALL*)(A...)> : function_trait<R(A...)> {
  static constexpr auto convention =
      calling_convention_by_abi_v<calling_conventions::Stdcall>;
};

template<typename R, typename... A>
struct function_traits<R(MYWR_THISCALL*)(A...)> : function_traits<R(A...)> {
  static constexpr auto convention =
      calling_convention_by_abi_v<calling_conventions::Thiscall>;
};

template<typename R, typename... A>
struct function_traits<R(MYWR_FASTCALL*)(A...)> : function_trait<R(A...)> {
  static constexpr auto convention =
      calling_convention_by_abi_v<calling_conventions::Fastcall>;
};
#endif

template<typename R, typename C, typename... A>
struct function_traits<R (C::*)(A...)> : function_trait<R(C*, A...)> {
  static constexpr auto convention =
      calling_convention_by_abi_v<calling_conventions::Thiscall>;
};

// Type aliases for simplified usage.

template<typename F>
using return_type_t = typename function_traits<F>::return_type;

template<typename F>
using arguments_t = typename function_traits<F>::arguments;

template<typename F, std::size_t I>
using argument_t = typename function_traits<F>::template argument<I>;

template<typename F>
constexpr auto args_count_v = function_traits<F>::args_count;

template<typename F>
constexpr auto convention_v = function_traits<F>::convention;

// Meta-function to concatenate calling convention with function signature.
template<typename R, calling_conventions C, typename... A>
struct concat_function_signature;

// Specializations of concat_function_signature.

template<typename R, typename... A>
struct concat_function_signature<R, calling_conventions::Cdecl, A...> {
  using signature = R(MYWR_CDECL*)(A...);
};

template<typename R, typename... A>
struct concat_function_signature<R, calling_conventions::Stdcall, A...> {
  using signature = R(MYWR_STDCALL*)(A...);
};

template<typename R, typename... A>
struct concat_function_signature<R, calling_conventions::Thiscall, A...> {
  using signature = R(MYWR_THISCALL*)(A...);
};

template<typename R, typename... A>
struct concat_function_signature<R, calling_conventions::Fastcall, A...> {
  using signature = R(MYWR_FASTCALL*)(A...);
};

template<typename R, typename... A>
struct concat_function_signature<R, calling_conventions::Win64, A...> {
  using signature = R (*)(A...);
};

// Type alias for simplified usage of concat_function_signature.
template<typename R, calling_conventions C, typename... A>
using concat_function_signature_t =
    typename concat_function_signature<R, C, A...>::signature;

template<typename R, calling_conventions C, typename Tuple>
struct concat_function_signature_by_args_tuple;

template<typename R, calling_conventions C, typename... A>
struct concat_function_signature_by_args_tuple<R, C, std::tuple<A...>> {
  using signature = concat_function_signature_t<R, C, A...>;
};

template<typename R, calling_conventions C, typename Tuple>
using concat_function_signature_by_args_tuple_t =
    typename concat_function_signature_by_args_tuple<R, C, Tuple>::signature;

} // namespace mywr::detail
