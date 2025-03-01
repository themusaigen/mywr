/**
 * @file version.hpp
 * @author themusaigen
 * @brief This file contains version information and utilities for the mywr
 * library.
 *
 * The version.hpp file provides version information, such as major, minor, and
 * patch versions, as well as utility functions for checking the version against
 * provided major, minor, and patch numbers.
 *
 * @copyright Copyright (c) 2025
 */
#pragma once

#include <cstdint>

// clang-format off

#define MYWR_MAJOR (1)
#define MYWR_MINOR (240)
#define MYWR_PATCH (2)
#define MYWR_VERSION_STR ("1.240.2")
#define MYWR_CHECK_VERSION(major, minor, patch) ((major >= MYWR_MAJOR) && (minor >= MYWR_MINOR) && (patch >= MYWR_PATCH))

// clang-format on

/**
 * @brief Namespace containing `mywr` library version utilities.
 */
namespace mywr::version {

/**
 * @brief Returns major version according to the SemVer (Version semantic).
 */
constexpr auto get_major() {
  return MYWR_MAJOR;
}

/**
 * @brief Returns minor version according to the SemVer (Version semantic).
 */
constexpr auto get_minor() -> uint32_t {
  return MYWR_MINOR;
}

/**
 * @brief Returns patch version according to the SemVer (Version semantic).
 */
constexpr auto get_patch() -> uint32_t {
  return MYWR_PATCH;
}

/**
 * @brief Returns full version stringified.
 */
constexpr auto get_version_str() {
  return MYWR_VERSION_STR;
}

/**
 * @brief Checks the version.
 */
constexpr auto check_version(uint32_t major, uint32_t minor, uint32_t patch) {
  return MYWR_CHECK_VERSION(major, minor, patch);
}

} // namespace mywr::version
