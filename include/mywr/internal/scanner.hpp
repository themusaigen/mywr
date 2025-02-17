/**
 * @file scanner.hpp
 * @brief Header file for memory scanning functionality in the mywr library.
 *
 * This file contains the definitions of classes and methods used for scanning
 * memory for specific byte patterns. It provides a base class for signature
 * patterns, as well as specific implementations for basic and IDA-style
 * signatures. The scanner class is responsible for performing the actual
 * memory scan within a specified range, initialized by a module handle or
 * module name.
 *
 * The classes in this file are designed to be used in the context of memory
 * hacking and reverse engineering, where identifying specific byte patterns
 * in memory is crucial. The library is intended to be cross-platform and
 * efficient, supporting various architectures and platforms.
 */
#pragma once

#include <shared/core/core.hpp>
#include <shared/core/address.hpp>

#include <internal/module.hpp>

#include <zstring_view.hpp>

#include <vector>

/**
 * @brief Namespace for memory scanning components in the mywr library.
 *
 * The mywr::scanner namespace contains classes and functions related to
 * scanning memory for specific byte patterns. It includes base classes for
 * defining signature patterns and a scanner class for executing the scan
 * within a specified memory range. The components in this namespace are
 * designed to be used in memory hacking and reverse engineering tasks,
 * providing efficient and flexible memory scanning capabilities.
 */
namespace mywr::scanner {

/**
 * @brief Base class for signature patterns used in memory scanning.
 *
 * This class provides the basic structure for storing a pattern and its
 * corresponding mask. The pattern is a sequence of bytes that the scanner will
 * attempt to match in memory. The mask is a sequence of booleans indicating
 * which bytes in the pattern are significant and should be matched exactly.
 */
class signature_base {
protected:
  // NOLINTBEGIN(*-non-private-member-variables-in-classes)

  /// Pattern of bytes to be matched.
  std::vector<byte_t> m_pattern;

  /// Mask indicating which bytes in the pattern are significant.
  std::vector<bool> m_mask;
  // NOLINTEND(*-non-private-member-variables-in-classes)

public:
  /**
   * @brief Get the pattern of bytes.
   * @return A vector of bytes representing the pattern.
   *
   * This method returns the pattern that was set during the construction of the
   * signature. The pattern is used during the scanning process to identify
   * matches in memory.
   */
  [[nodiscard]] auto pattern() const noexcept -> auto& {
    return m_pattern;
  }

  /**
   * @brief Get the mask for the pattern.
   * @return A vector of booleans representing the mask.
   *
   * This method returns the mask associated with the pattern. The mask
   * indicates which bytes in the pattern are significant and should be matched
   * exactly during the scanning process. A value of `true` in the mask means
   * the corresponding byte in the pattern is significant.
   */
  [[nodiscard]] auto mask() const noexcept -> auto& {
    return m_mask;
  }
};

/**
 * @brief Represents a basic signature with a pattern and mask.
 *
 * This class constructs a signature from a given pattern and mask. The pattern
 * is a string of characters, and the mask is a string where 'x' indicates a
 * significant byte and any other character indicates a wildcard byte.
 */
class basic_signature final : public signature_base {
public:
  /**
   * @brief Construct a new basic signature object.
   * @param pattern The pattern string.
   * @param mask The mask string.
   *
   * This constructor initializes the basic signature with a pattern and a mask.
   * The pattern is a string of characters representing the bytes to be matched,
   * and the mask is a string where 'x' indicates that the corresponding byte in
   * the pattern is significant and should be matched exactly.
   */
  // NOLINTBEGIN(*-swappable-parameters)
  basic_signature(const char* pattern, const char* mask) {
    // NOLINTEND(*-swappable-parameters)
    // NOLINTBEGIN(*-pointer-arithmetic)
    for (size_t i = 0U; static_cast<bool>(mask[i]); ++i) {
      if (mask[i] == 'x') {
        m_pattern.push_back(static_cast<byte_t>(pattern[i]));
      } else {
        m_pattern.push_back(0x00);
      }

      m_mask.push_back(mask[i] == 'x');
    }
    // NOLINTEND(*-pointer-arithmetic)
  }
};

/**
 * @brief Represents an IDA-style signature.
 *
 * This class constructs a signature from a pattern string using IDA-style
 * wildcards. In this format, '?' represents a wildcard byte that can match any
 * value, and spaces are ignored.
 */
class ida_signature final : public signature_base {
public:
  /**
   * @brief Construct a new IDA signature object.
   * @param pattern The pattern string with IDA-style wildcards.
   *
   * This constructor initializes the IDA signature with a pattern string. The
   * pattern can contain hexadecimal byte values and '?' as wildcards. Spaces in
   * the pattern are ignored. The constructor processes the pattern to create
   * the corresponding byte pattern and mask.
   */
  ida_signature(mpt::zstring_view pattern) {
    m_pattern.reserve(pattern.size());
    m_mask.reserve(pattern.size());

    size_t index{0U};
    while (index < pattern.size()) {
      const char character{pattern[index]};
      if (character == ' ') {
        ++index;
        continue;
      }

      if (character == '?') {
        m_pattern.push_back(0x00);
        m_mask.push_back(false);
      } else {
        const char high{character};
        const char low{pattern[index + 1]};

        constexpr int kHexDecimalOffset = 0xA;

        byte_t byte{0x00};

        if (high >= '0' && high <= '9') {
          byte |= static_cast<byte_t>((high - '0') << 4);
        } else if (high >= 'A' && high <= 'F') {
          byte |= static_cast<byte_t>((high - 'A' + kHexDecimalOffset) << 4);
        }

        if ((low >= '0') && (low <= '9')) {
          byte |= static_cast<byte_t>(low - '0');
        } else if ((low >= 'A') && (low <= 'F')) {
          byte |= static_cast<byte_t>(low - 'A' + kHexDecimalOffset);
        }

        m_pattern.push_back(byte);
        m_mask.push_back(true);
      }

      index += 2;
    }
  }
};

/**
 * @brief A class for scanning memory for specific byte patterns.
 *
 * The scanner class is responsible for searching a specified memory range for a
 * given signature pattern. It can be initialized with a module handle or a
 * module name, and it provides a method to perform the scan and return the
 * address of the first match.
 */
class scanner {
private:
  /// Start address of the memory range to scan.
  address m_start{};

  /// End address of the memory range to scan.
  address m_end{};

public:
  /**
   * @brief Construct a new scanner object using a module handle.
   * @param handle The module handle to initialize the scanner.
   *
   * This constructor initializes the scanner with a module handle. It sets the
   * start and end addresses of the memory range to be scanned based on the
   * module's allocation base and image size.
   */
  scanner(const address& handle) noexcept {
    initialize(handle);
  }

  /**
   * @brief Construct a new scanner object using a module name.
   * @param name The module name to initialize the scanner.
   *
   * This constructor initializes the scanner with a module name. It retrieves
   * the module handle using the name and sets the start and end addresses of
   * the memory range to be scanned.
   */
  scanner(mpt::zstring_view name) noexcept {
    initialize(name);
  }

  /**
   * @brief Construct a new scanner object using a wide string module name.
   * @param name The wide string module name to initialize the scanner.
   *
   * This constructor initializes the scanner with a wide string module name. It
   * retrieves the module handle using the name and sets the start and end
   * addresses of the memory range to be scanned.
   */
  scanner(mpt::wzstring_view name) noexcept {
    initialize(name);
  }

  /**
   * @brief Initialize the scanner with a module handle.
   * @param handle The module handle to initialize the scanner.
   * @return True if initialization is successful, false otherwise.
   *
   * This method initializes the scanner with a module handle. It sets the start
   * and end addresses of the memory range to be scanned based on the module's
   * allocation base and image size. Returns true if the initialization is
   * successful.
   */
  auto initialize(const address& handle) noexcept -> bool {
    auto module_allocation_base = module::get_module_allocation_base(handle);
    auto module_image_size =
        module::get_module_image_size(module_allocation_base);

    if (module_allocation_base && module_image_size) {
      m_start = module_allocation_base;
      m_end   = m_start + module_image_size;
      return true;
    }
    return false;
  }

  /**
   * @brief Initialize the scanner with a module name.
   * @param name The module name to initialize the scanner.
   * @return True if initialization is successful, false otherwise.
   *
   * This method initializes the scanner with a module name. It retrieves the
   * module handle using the name and sets the start and end addresses of the
   * memory range to be scanned. Returns true if the initialization is
   * successful.
   */
  auto initialize(mpt::zstring_view name) noexcept -> bool {
    return initialize(module::get_module_handle(name));
  }

  /**
   * @brief Initialize the scanner with a wide string module name.
   * @param name The wide string module name to initialize the scanner.
   * @return True if initialization is successful, false otherwise.
   *
   * This method initializes the scanner with a wide string module name. It
   * retrieves the module handle using the name and sets the start and end
   * addresses of the memory range to be scanned. Returns true if the
   * initialization is successful.
   */
  auto initialize(mpt::wzstring_view name) noexcept -> bool {
    return initialize(module::get_module_handle(name));
  }

  /**
   * @brief Scan the memory range for a specific signature pattern.
   * @param signature The signature pattern to search for.
   * @return The address of the first match, or nullptr if no match is found.
   *
   * This method scans the memory range specified by the start and end addresses
   * for the given signature pattern. It returns the address of the first match
   * found, or nullptr if no match is found. The scan is performed byte by byte,
   * and the pattern and mask are used to determine matches.
   */
  [[nodiscard]] auto scan(const signature_base& signature) noexcept -> address {
    if (!m_start || !m_end) {
      return nullptr;
    }

    auto* current{m_start.pointer<byte_t>()};
    auto* end{m_end.pointer<byte_t>()};

    const auto& pattern = signature.pattern();
    const auto& mask    = signature.mask();

    // NOLINTBEGIN(*-pointer-arithmetic)
    while (current < end) {
      bool match{true};
      for (size_t i = 0U; i < pattern.size(); ++i) {
        if (mask[i] && *current != pattern[i]) {
          match = false;
          break;
        }
      }

      if (match) {
        return current;
      }

      ++current;
    }
    // NOLINTEND(*-pointer-arithmetic)

    return nullptr;
  }
};
} // namespace mywr::scanner
