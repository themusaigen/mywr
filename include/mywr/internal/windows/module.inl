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
 * @brief Retrieves the module handle for the current executable using the
 *        Windows API function GetModuleHandleA.
 *
 * This function uses the Windows API function GetModuleHandleA to retrieve the
 * module handle for the current executable. It passes a null pointer to the
 * function, which retrieves the handle of the executable module that started
 * the current process. The function is marked as `noexcept` to indicate that
 * it does not throw any exceptions.
 *
 * @return The address of the module handle for the current executable. If the
 *         function fails to retrieve the handle, it returns an invalid
 *         address.
 *
 * @note This function is specific to the Windows platform and uses the
 *       Windows API function GetModuleHandleA.
 */
[[nodiscard]] inline auto get_executable_module_handle() noexcept -> address {
  return GetModuleHandleA(nullptr);
}

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

/**
 * @brief Retrieves the base address of the memory allocation for the specified
 *        module handle using the Windows API function VirtualQuery.
 *
 * This function uses the Windows API function VirtualQuery to retrieve the base
 * address of the memory allocation for the module associated with the given
 * handle. The function is marked as `noexcept` to indicate that it does not
 * throw any exceptions.
 *
 * @param handle The handle of the module.
 * @return The base address of the memory allocation. If the handle is invalid,
 *         the function returns an invalid address.
 */
[[nodiscard]] inline auto
    get_module_allocation_base(const address& handle) noexcept -> address {
  MEMORY_BASIC_INFORMATION info{};
  if (VirtualQuery(handle, &info, sizeof(info)) == sizeof(info)) {
    return info.AllocationBase;
  }
  return nullptr;
}

/**
 * @brief Retrieves the size of the image for the specified module allocation
 *        base using the Windows API function IMAGE_DOS_HEADER and
 *        IMAGE_NT_HEADERS.
 *
 * This function uses the Windows API structures IMAGE_DOS_HEADER and
 * IMAGE_NT_HEADERS to retrieve the size of the image for the module associated
 * with the given allocation base. The function is marked as `noexcept` to
 * indicate that it does not throw any exceptions.
 *
 * @param allocation_base The base address of the module's memory allocation.
 * @return The size of the image. If the allocation base is invalid, the
 *         function returns 0.
 */
[[nodiscard]] inline auto
    get_module_image_size(const address& allocation_base) noexcept -> address {
  // NOLINTBEGIN(*-int-to-ptr)
  auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(address_t{allocation_base});
  auto* pex = reinterpret_cast<IMAGE_NT_HEADERS*>(
      reinterpret_cast<address_t>(dos) + static_cast<ULONG>(dos->e_lfanew));
  // NOLINTEND(*-int-to-ptr)

  if (pex->Signature != IMAGE_NT_SIGNATURE) {
    return nullptr;
  }

  return pex->OptionalHeader.SizeOfImage;
}
} // namespace mywr::module
