/**
 * @file module.hpp
 * @brief Contains declarations for functions related to module handling.
 *
 * This file provides declarations for functions that retrieve module handles
 * based on their names. The implementation is platform-specific and is included
 * based on the defined preprocessor macro.
 */
#pragma once

#include <shared/core/os.hpp>
#include <shared/core/address.hpp>

#include <zstring_view.hpp>

namespace mywr::module {
/**
 * @brief Retrieves the module handle for the specified module name.
 *
 * This function uses the platform-specific method to retrieve the module handle
 * for the module with the given name. The name can be specified as a narrow
 * string (zstring_view) or a wide string (wzstring_view). The function is
 * marked as `noexcept` to indicate that it does not throw any exceptions.
 *
 * @param name The name of the module.
 * @return The address of the module handle. If the module is not found, the
 *         function returns an invalid address.
 */
[[nodiscard]] inline auto get_module_handle(mpt::zstring_view name) noexcept
    -> address;

/**
 * @brief Retrieves the module handle for the specified module name.
 *
 * This function uses the platform-specific method to retrieve the module handle
 * for the module with the given name. The name can be specified as a wide
 * string (wzstring_view). The function is marked as `noexcept` to indicate that
 * it does not throw any exceptions.
 *
 * @param name The name of the module.
 * @return The address of the module handle. If the module is not found, the
 *         function returns an invalid address.
 */
[[nodiscard]] inline auto get_module_handle(mpt::wzstring_view name) noexcept
    -> address;
} // namespace mywr::module

#if defined(MYWR_WINDOWS)
  #include <internal/windows/module.inl>
#endif
