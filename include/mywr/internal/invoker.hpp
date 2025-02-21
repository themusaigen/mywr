/**
 * @file invoker.hpp
 * @brief Provides a function invocation mechanism for different function types.
 *
 * @details This header file is part of the `mywr` library. It defines a
 * mechanism for invoking functions with various types and calling conventions.
 * The file utilizes meta-programming utilities to determine the appropriate
 * calling convention and function signature for a given function type. It is
 * designed to be part of a header-only library, as indicated by the use of
 * `#pragma once` to prevent multiple inclusions.
 *
 * The functionality is encapsulated within the `mywr::invoker` namespace, which
 * suggests that it is part of a larger library or framework related to function
 * invocation or manipulation.
 *
 * The file includes several headers from the `shared` directory, which likely
 * provide core utilities, calling convention definitions, function traits, and
 * other necessary components for the invocation mechanism.
 *
 * The main component of this file is the `invoke` function template, which
 * determines the function signature using the calling convention and arguments,
 * and then invokes the function at the given memory address using the
 * determined signature.
 *
 * @author themusaigen
 */
#pragma once

#include <shared/core/address.hpp>

#include <shared/calling_conventions.hpp>
#include <shared/function_traits.hpp>
#include <shared/utility.hpp>

#include <utility>

namespace mywr::invoker {

/**
 * @brief Invokes a function at a given memory address with specified arguments.
 *
 * @tparam F The function type.
 * @tparam In The type of the function address input.
 * @tparam A The types of the arguments to pass to the function.
 * @param fun The memory address of the function to invoke.
 * @param args The arguments to pass to the function.
 * @return The return type of the function, as determined by
 * `detail::return_type_t<F>`.
 *
 * @details This function template uses meta-programming to determine the
 * function signature based on the calling convention and arguments. It then
 * invokes the function at the specified memory address using the determined
 * signature. The function is marked `noexcept` to indicate that it does not
 * throw exceptions.
 */
template<typename F, typename In, typename... A>
inline auto invoke(In fun, A&&... args) noexcept -> detail::return_type_t<F> {
  using namespace detail;

  // Determine the function signature using the calling convention and
  // arguments.
  using signature_t =
      concat_function_signature_by_args_tuple_t<return_type_t<F>,
                                                convention_v<F>,
                                                arguments_t<F>>;

  // Invoke the function at the given memory address using the determined
  // signature.
  return reinterpret_cast<signature_t>(utility::get_function_address(fun))(
      std::forward<A>(args)...);
}
} // namespace mywr::invoker
