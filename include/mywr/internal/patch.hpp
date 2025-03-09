/**
 * @file patch.hpp
 * @brief Defines classes for memory patching operations.
 *
 * This file contains the implementation of scoped_patch_unit, scoped_patch,
 * and patch_builder classes, which together provide a robust and flexible
 * system for applying and managing memory patches in a safe, RAII-compliant
 * manner.
 *
 */
#pragma once

#include <shared/core/core.hpp>
#include <shared/core/address.hpp>

#include <internal/protect.hpp>
#include <internal/llmo.hpp>
#include <internal/module.hpp>

#include <zstring_view.hpp>

#include <type_traits>
#include <vector>
#include <string>

namespace mywr::patch {

/**
 * @brief Represents a single unit of a memory patch.
 *
 * This class encapsulates the details of a single patch operation,
 * including the target address, replacement bytes, and original bytes.
 */
class scoped_patch_unit {
  using bytes_t = std::vector<byte_t>;

private:
  address m_address{};   ///< The target address for the patch
  bytes_t m_replacement; ///< The bytes to replace at the target address
  bytes_t m_original;    ///< The original bytes at the target address

public:
  /**
   * @brief Default constructor.
   */
  scoped_patch_unit() noexcept = default;

  /**
   * @brief Adjusts the target address based on the module's base address.
   * @param module The base address of the module.
   */
  void at_module(address module) noexcept {
    m_address += module;
  }

  /**
   * @brief Sets or adjusts the target address for the patch.
   * @param address The address to set or add to the current address.
   */
  void target(address address) noexcept {
    if (address_t{m_address} == 0U) {
      m_address = address;
    } else {
      m_address += address;
    }
  }

  /**
   * @brief Sets the replacement bytes for the patch.
   * @param replacement The bytes to use as replacement.
   */
  void replace_with(bytes_t replacement) noexcept {
    m_replacement = std::move(replacement);
  }

  /**
   * @brief Sets the original bytes for the patch.
   * @param original The original bytes at the target address.
   */
  void with_original(bytes_t original) noexcept {
    m_original = std::move(original);
  }

  /**
   * @brief Installs the patch at the target address.
   * @return true if the patch was successfully installed, false otherwise.
   */
  auto install() noexcept -> bool {
    if (!m_address) {
      return false;
    }

    if (m_replacement.empty()) {
      return false;
    }

    using namespace protect;

    scoped_protect protect(
        m_address, protection::ReadWriteExecute, m_replacement.size());

    if (m_original.empty()) {
      m_original.resize(m_replacement.size());

      // true = got an error.
      if (llmo::copy(m_original.data(), m_address, m_original.size(), false)) {
        return false;
      }
    }

    return !llmo::copy(
        m_address, m_replacement.data(), m_replacement.size(), false);
  }

  /**
   * @brief Removes the patch, restoring the original bytes.
   * @return true if the patch was successfully removed, false otherwise.
   */
  auto remove() noexcept -> bool {
    if (!m_address) {
      return false;
    }

    if (m_original.empty()) {
      return false;
    }

    return !llmo::copy(m_address, m_original.data(), m_original.size());
  }
};

/**
 * @brief Manages a collection of patch units.
 *
 * This class provides RAII-compliant management of multiple patch units,
 * automatically removing patches when the object goes out of scope.
 */
class scoped_patch {
private:
  std::vector<scoped_patch_unit> m_units; ///< Collection of patch units

public:
  /**
   * @brief Default constructor.
   */
  scoped_patch() noexcept = default;

  /**
   * @brief Copy constructor (deleted).
   */
  scoped_patch(const scoped_patch&) noexcept = delete;

  /**
   * @brief Move constructor.
   * @param rhs The scoped_patch to move from.
   */
  scoped_patch(scoped_patch&& rhs) noexcept
      : m_units(std::move(rhs.m_units)) {}

  /**
   * @brief Destructor. Automatically removes all patches.
   */
  ~scoped_patch() noexcept {
    remove();
  }

  /**
   * @brief Copy assignment operator (deleted).
   */
  auto operator=(const scoped_patch&) -> scoped_patch& = delete;

  /**
   * @brief Move assignment operator.
   * @param rhs The scoped_patch to move from.
   * @return Reference to this scoped_patch.
   */
  auto operator=(scoped_patch&& rhs) noexcept -> scoped_patch& {
    m_units = std::move(rhs.m_units);
    return *this;
  }

  /**
   * @brief Installs all patch units.
   * @return true if all patches were successfully installed, false otherwise.
   */
  auto install() noexcept -> bool {
    for (auto& unit : m_units) {
      if (!unit.install()) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Removes all patch units.
   * @return true if all patches were successfully removed, false otherwise.
   */
  auto remove() noexcept -> bool {
    for (auto& unit : m_units) {
      if (!unit.remove()) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Returns the number of patch units in this scoped_patch.
   * @return The number of patch units.
   */
  [[nodiscard]] auto size() const noexcept -> size_t {
    return m_units.size();
  }

  /**
   * @brief Adds a new patch unit to the collection.
   * @param unit The patch unit to add.
   */
  void add(scoped_patch_unit unit) noexcept {
    m_units.push_back(std::move(unit));
  }
};

/**
 * @brief A builder class for creating and configuring patches.
 *
 * This class provides a fluent interface for building patches, allowing for
 * easy configuration of patch units with method chaining.
 */
class patch_builder {
private:
  scoped_patch      m_patch; ///< The scoped patch being built
  scoped_patch_unit m_unit;  ///< The current patch unit being configured

public:
  /**
   * @brief Creates a new patch_builder instance.
   * @return A new patch_builder object.
   *
   * This static method serves as a factory for creating patch_builder objects.
   *
   * Example usage:
   * @code
   * auto builder = patch_builder::create();
   * @endcode
   */
  static auto create() noexcept -> patch_builder {
    return patch_builder{};
  }

  /**
   * @brief Sets the module for the current patch unit.
   * @param modname The name of the module.
   * @return Reference to this patch_builder for method chaining.
   *
   * This method sets the module for the current patch unit.
   *
   * @note This method will only have an effect if a valid module handle is
   * obtained.
   *
   * Example usage:
   * @code
   * builder.at_module("kernel32.dll");
   * @endcode
   */
  auto at_module(mpt::zstring_view modname) noexcept -> patch_builder& {
    auto handle{module::get_module_handle(modname)};
    if (handle) {
      m_unit.at_module(std::move(handle));
    }
    return *this;
  }

  /**
   * @brief Sets the module for the current patch unit.
   * @param modname The name of the module.
   * @return Reference to this patch_builder for method chaining.
   *
   * This method sets the module for the current patch unit.
   *
   * @note This method will only have an effect if a valid module handle is
   * obtained.
   *
   * Example usage:
   * @code
   * builder.at_module(L"kernel32.dll");
   * @endcode
   */
  auto at_module(mpt::wzstring_view modname) noexcept -> patch_builder& {
    auto handle{module::get_module_handle(modname)};
    if (handle) {
      m_unit.at_module(std::move(handle));
    }
    return *this;
  }

  /**
   * @brief Sets the target address for the current patch unit.
   * @param address The target address.
   * @return Reference to this patch_builder for method chaining.
   *
   * This method sets or updates the target address for the current patch unit.
   *
   * Example usage:
   * @code
   * builder.target(0x12345678);
   * @endcode
   */
  auto target(address address) noexcept -> patch_builder& {
    m_unit.target(std::move(address));
    return *this;
  }

  /**
   * @brief Sets the replacement bytes for the current patch unit.
   * @param replacement The replacement bytes.
   * @return Reference to this patch_builder for method chaining.
   *
   * This method sets the replacement bytes for the current patch unit.
   *
   * Example usage:
   * @code
   * builder.replace_with(std::vector<byte_t>{0x90, 0x90, 0x90});
   * @endcode
   */
  auto replace_with(std::vector<byte_t> replacement) noexcept
      -> patch_builder& {
    m_unit.replace_with(std::move(replacement));
    return *this;
  }

  /**
   * @brief Sets the replacement value for the current patch unit.
   * @tparam T The type of the replacement value.
   * @param value The replacement value.
   * @return Reference to this patch_builder for method chaining.
   *
   * This template method allows setting the replacement value for various
   * types. It handles std::string differently from other types.
   *
   * @note For std::string, it converts the string to bytes.
   * @note For other types, it performs a byte-wise copy of the value.
   *
   * Example usage:
   * @code
   * builder.replace_with(42);  // For an integer
   * builder.replace_with(std::string("Hello"));  // For a string
   * @endcode
   */
  template<typename T>
  auto replace_with(const T& value) noexcept -> patch_builder& {
    // Check if T is a string type.
    if constexpr (std::is_same_v<T, std::string>) {
      // Convert string to bytes.
      std::vector<byte_t> replacement(value.begin(), value.end());

      // Move vector.
      replace_with(std::move(replacement));
    } else {
      // Convert to bytes.
      std::vector<byte_t> replacement(sizeof(T));
      memcpy(replacement.data(), &value, sizeof(T));

      // Move vector.
      replace_with(std::move(replacement));
    }

    return *this;
  }

  /**
   * @brief Sets the original bytes for the current patch unit.
   * @param original The original bytes.
   * @return Reference to this patch_builder for method chaining.
   *
   * This method sets the original bytes for the current patch unit.
   *
   * Example usage:
   * @code
   * builder.with_original(std::vector<byte_t>{0x55, 0x8B, 0xEC});
   * @endcode
   */
  auto with_original(std::vector<byte_t> original) noexcept -> patch_builder& {
    m_unit.with_original(std::move(original));
    return *this;
  }

  /**
   * @brief Sets the original value for the current patch unit.
   * @tparam T The type of the original value.
   * @param value The original value.
   * @return Reference to this patch_builder for method chaining.
   *
   * This template method allows setting the original value for various types.
   * It handles std::string differently from other types.
   *
   * @note For std::string, it converts the string to bytes.
   * @note For other types, it performs a byte-wise copy of the value.
   *
   * @warning Be cautious with complex types that have non-trivial copy
   * semantics or contain pointers, as the byte-wise copy may not capture the
   * full state.
   *
   * Example usage:
   * @code
   * builder.with_original(42);  // For an integer
   * builder.with_original(std::string("Hello"));  // For a string
   * @endcode
   */
  template<typename T>
  auto with_original(const T& value) noexcept -> patch_builder& {
    // Check if T is a string type.
    if constexpr (std::is_same_v<T, std::string>) {
      // Convert string to bytes.
      std::vector<byte_t> original(value.begin(), value.end());

      // Move vector.
      with_original(std::move(original));
    } else {
      // Convert to bytes.
      std::vector<byte_t> original(sizeof(T));
      memcpy(original.data(), &value, sizeof(T));

      // Move vector.
      with_original(std::move(original));
    }

    return *this;
  }

  /**
   * @brief Finalizes the current patch unit and prepares for the next one.
   * @return Reference to this patch_builder for method chaining.
   *
   * This method adds the current patch unit to the scoped_patch and resets
   * the current unit, allowing for the configuration of a new patch unit.
   *
   * Example usage:
   * @code
   * builder.target(0x12345678).replace_with(0x90).next()
   *        .target(0x87654321).replace_with(0x91);
   * @endcode
   */
  auto next() noexcept -> patch_builder& {
    m_patch.add(m_unit);
    m_unit = scoped_patch_unit{};
    return *this;
  }

  /**
   * @brief Finalizes the patch building process and returns the built
   * scoped_patch.
   * @return The built scoped_patch.
   *
   * This method finalizes the current patch unit (if any), adds it to the
   * scoped_patch, and returns the completed scoped_patch object. After calling
   * this method, the patch_builder object should not be used further.
   *
   * Example usage:
   * @code
   * auto patch = builder.target(0x12345678).replace_with(0x90).pack();
   * patch.install();
   * @endcode
   */
  auto pack() noexcept -> scoped_patch {
    next();
    return std::move(m_patch);
  }
};

} // namespace mywr::patch
