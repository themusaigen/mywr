/**
 * @file module.inl
 * @brief Contains platform-specific implementations for module handling
 * functions.
 *
 * This file provides platform-specific implementations for the module handling
 * functions declared in the mywr::module namespace. The implementations are
 * based on the Windows API functions GetModuleHandleA and GetModuleHandleW.
 */
#pragma once

#include <shared/core/osheaders.hpp>

#include <internal/module.hpp>

#include <zstring_view.hpp>

namespace mywr::module {
/**
 * @brief Retrieves the module handle for the specified module name using the
 *        Windows API function GetModuleHandleA.
 *
 * This function uses the Windows API function GetModuleHandleA to retrieve the
 * module handle for the module with the given name. The name can be specified
 * as a narrow string (zstring_view). The function is marked as `noexcept` to
 * indicate that it does not throw any exceptions.
 *
 * @param name The name of the module.
 * @return The address of the module handle. If the module is not found, the
 *         function returns an invalid address.
 */
[[nodiscard]] inline auto get_module_handle(mpt::zstring_view name) noexcept
    -> address {
  return GetModuleHandleA(name.data());
}

/**
 * @brief Retrieves the module handle for the specified module name using the
 *        Windows API function GetModuleHandleW.
 *
 * This function uses the Windows API function GetModuleHandleW to retrieve the
 * module handle for the module with the given name. The name can be specified
 * as a wide string (wzstring_view). The function is marked as `noexcept` to
 * indicate that it does not throw any exceptions.
 *
 * @param name The name of the module.
 * @return The address of the module handle. If the module is not found, the
 *         function returns an invalid address.
 */
[[nodiscard]] inline auto get_module_handle(mpt::wzstring_view name) noexcept
    -> address {
  return GetModuleHandleW(name.data());
}
} // namespace mywr::module
