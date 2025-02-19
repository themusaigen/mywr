/**
 * @file invoker.hpp
 * @brief Provides a function invocation mechanism for different function types.
 *
 * @details The file includes a meta-programming utility to determine the
 * calling convention of a function and a namespace for invoking functions at a
 * given memory address.
 *
 * @author themusaigen
 */
#pragma once

#include <shared/core/address.hpp>

#include <shared/calling_conventions.hpp>
#include <shared/function_traits.hpp>

#include <utility>
#include <functional>

namespace mywr::detail {
template<typename F>
struct invoker {
  template<typename... A>
  static auto invoke(const address& fun, A&&... args) noexcept
      -> return_type_t<F> {
    // Determine the function signature using the calling convention and
    // arguments.
    using signature_t =
        concat_function_signature_t<return_type_t<F>, convention_v<F>, A...>;

    // Invoke the function at the given memory address using the determined
    // signature.
    return std::invoke(reinterpret_cast<signature_t>(address_t{fun}),
                       std::forward<A>(args)...);
  }
};
} // namespace mywr::detail

namespace mywr::invoker {
template<typename F, typename... A>
inline auto invoke(const address& fun, A&&... args) noexcept
    -> detail::return_type_t<F> {
  // Invoke the function using the invoker meta-function.
  return detail::invoker<F>::template invoke<A...>(fun,
                                                   std::forward<A>(args)...);
}
} // namespace mywr::invoker
