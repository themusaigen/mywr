/**
 * @file function_traits.hpp
 * @brief Provides template meta-programming utilities for extracting and
 * manipulating function type information.
 * @author themusaigen
 */
#pragma once

#include <shared/abi.hpp>
#include <shared/calling_conventions.hpp>

#include <cstddef>
#include <tuple>
#include <functional>

namespace mywr::detail {

template<typename F>
struct function_trait;

template<typename F>
struct function_traits;

// Implementation of function_trait.
template<typename R, typename... A>
struct function_trait<R(A...)> {
  using return_type = R;
  using arguments   = std::tuple<A...>;

  template<std::size_t I>
  using argument = std::tuple_element<I, std::tuple<A...>>;

  static constexpr auto args_count = sizeof...(A);
};

// Additional specializations for std::function and different calling
// conventions.

template<typename R, typename... A>
struct function_traits<R(A...)> : function_trait<R(A...)> {
  static constexpr auto convention =
      calling_convention_by_abi_v<calling_conventions::Cdecl>;
};

template<typename R, typename... A>
struct function_traits<std::function<R(A...)>> : function_trait<R(A...)> {
  static constexpr auto convention =
      calling_convention_by_abi_v<calling_conventions::Cdecl>;
};

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
  using signature = R(A...);
};

// Type alias for simplified usage of concat_function_signature.
template<typename R, calling_conventions C, typename... A>
using concat_function_signature_t =
    typename concat_function_signature<R, C, A...>::signature;

} // namespace mywr::detail
