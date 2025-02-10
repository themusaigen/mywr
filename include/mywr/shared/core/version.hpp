#ifndef MYWR_SHARED_VERSION_HPP_
#define MYWR_SHARED_VERSION_HPP_

#include <cstdint>

// clang-format off

#define MYWR_MAJOR (1)
#define MYWR_MINOR (161)
#define MYWR_PATCH (0)
#define MYWR_VERSION_STR ("1.161.0")
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

#endif
