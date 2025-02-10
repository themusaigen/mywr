/**
 * @file protection_flags.hpp
 * @author themusaigen
 * @brief Module that contains protection flags constants.
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef MYWR_SHARED_PROTECT_PROTECTION_FLAGS_HPP_
#define MYWR_SHARED_PROTECT_PROTECTION_FLAGS_HPP_

#include <shared/core/os.hpp>

#include <cstdint>

/**
 * @brief Namespace that contating protection utilities.
 */
namespace mywr::protect {
class protection {
public:
  enum Enum : std::uint8_t {
    None     = (1 << 0),
    NoAccess = (1 << 1),
    Read     = (1 << 2),
    Write    = (1 << 3),
    Execute  = (1 << 4),

    ReadWrite        = (Read | Write),
    ReadExecute      = (Read | Execute),
    ReadWriteExecute = (Read | Write | Execute)
  };
};

/**
 * @brief Transforms system protection constant to `mywr` constant.
 */
static auto to_protection_constant(std::uint32_t protect) -> protection::Enum;

/**
 * @brief Transforms `mywr` protection constant to system protection constant.
 */
static auto from_protection_constant(protection::Enum protect) -> std::uint32_t;

} // namespace mywr::protect

#if defined(MYWR_WINDOWS)
  #include <shared/windows/protection_flags.hpp>
#endif

#endif
