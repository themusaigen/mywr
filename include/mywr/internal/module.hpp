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

/**
 * @brief This namespace contains declarations for functions related to module
 * handling.
 *
 * The functions in this namespace are designed to help developers retrieve
 * module handles based on their names. They can be used to interact with
 * modules in a platform-independent manner.
 *
 * @note The implementation of the functions in this namespace is
 * platform-specific and is included based on the defined preprocessor macro.
 *
 * @note It is recommended to use platform-independent methods for retrieving
 * module handles whenever possible. This will help ensure that your code
 * remains portable and compatible across different operating systems.
 */
namespace mywr::module {
/**
 * @brief Retrieves the module handle for the currently executing executable.
 *
 * This function uses the platform-specific method to retrieve the module handle
 * for the currently executing executable. The function is marked as `noexcept`
 * to indicate that it does not throw any exceptions.
 *
 * @return The address of the module handle for the currently executing
 * executable. If the module handle cannot be retrieved, the function returns an
 * invalid address.
 *
 * @note This function is platform-specific and may not be available on all
 * operating systems. It is recommended to use platform-independent methods for
 * retrieving module handles whenever possible.
 */
[[nodiscard]] inline auto get_executable_module_handle() noexcept -> address;

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

/**
 * @brief Retrieves the base address of the memory allocation for the specified
 *        module handle.
 *
 * This function uses the platform-specific method to retrieve the base address
 * of the memory allocation for the module associated with the given handle. The
 * function is marked as `noexcept` to indicate that it does not throw any
 * exceptions.
 *
 * @param handle The handle of the module.
 * @return The base address of the memory allocation. If the handle is invalid,
 *         the function returns an invalid address.
 */
[[nodiscard]] inline auto
    get_module_allocation_base(const address& handle) noexcept -> address;

/**
 * @brief Retrieves the size of the image for the specified module allocation
 *        base.
 *
 * This function uses the platform-specific method to retrieve the size of the
 * image for the module associated with the given allocation base. The function
 * is marked as `noexcept` to indicate that it does not throw any exceptions.
 *
 * @param allocation_base The base address of the module's memory allocation.
 * @return The size of the image. If the allocation base is invalid, the
 *         function returns 0.
 */
[[nodiscard]] inline auto
    get_module_image_size(const address& allocation_base) noexcept -> address;
} // namespace mywr::module

#if defined(MYWR_WINDOWS)
  #include <internal/windows/module.inl>
#endif
