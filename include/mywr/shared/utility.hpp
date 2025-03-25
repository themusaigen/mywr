/**
 * @file utility.hpp
 * @brief Provides utility functions for type casting and address manipulation.
 *
 * @details This header file is part of the `mywr` library. It defines utility
 * functions that facilitate type casting and address manipulation, which are
 * commonly used in low-level programming tasks. The file includes the
 * `address.hpp` header, which defines the `address_t` type used for
 * representing memory addresses.
 *
 * The main components of this file are:
 *
 * - `force_cast`: A template function that performs a type-safe cast between
 *   two types of the same size.
 *
 * - `get_function_address`: A template function that retrieves the memory
 *   address of a function or object.
 *
 * - `get_relative_address`: A function that calculates the relative address
 *   between two memory locations.
 *
 * - `restore_absolute_address`: A function that restores an absolute address
 *   from a relative address.
 *
 * These functions are encapsulated within the `mywr::utility` namespace,
 * indicating that they are intended for general utility purposes within the
 * library.
 *
 * @note The `force_cast` function is a low-level utility that should be used
 * with caution, as it bypasses type safety and can lead to undefined behavior
 * if used incorrectly.
 *
 * @see address.hpp for the definition of the `address_t` type.
 */
#pragma once

#include <shared/core/address.hpp>

namespace mywr::utility {
/**
 * @brief Performs a type-safe cast between two types of the same size.
 *
 * @tparam Out The output type.
 * @tparam In The input type.
 * @param input The input value to be cast.
 * @return The cast value as the output type.
 *
 * @note This function uses `memcpy` to copy the bits from the input to the
 * output, ensuring that the cast is performed without invoking any constructors
 * or destructors. The function includes a static assertion to ensure that the
 * input and output types have the same size.
 */
template<typename Out, typename In>
constexpr auto force_cast(In input) -> Out {
  static_assert(sizeof(In) == sizeof(Out));

  Out out{};
  memcpy(&out, &input, sizeof(Out));

  return out;
}

/**
 * @brief Retrieves the memory address of a function or object.
 *
 * @tparam In The input type.
 * @param input The input value whose address is to be retrieved.
 * @return The memory address as an `address_t` type.
 *
 * @note This function uses `force_cast` to convert the input to an `address_t`
 * type, which represents a memory address.
 */
template<typename In>
constexpr auto get_function_address(In input) -> address_t {
  return force_cast<address_t>(input);
}

/**
 * @brief Calculates the relative address between two memory locations.
 *
 * @param destination The destination address.
 * @param source The source address.
 * @param size The size of the instruction or memory block.
 * @return The calculated relative address.
 */
constexpr auto get_relative_address(const address& destination,
                                    const address& source,
                                    size_t         size) -> address {
  return (destination - source).value() - size;
}

/**
 * @brief Restores an absolute address from a relative address.
 *
 * @param relative The relative address.
 * @param base The base address.
 * @param size The size of the instruction or memory block.
 * @return The restored absolute address.
 */
constexpr auto restore_absolute_address(const address& relative,
                                        const address& base,
                                        size_t         size) -> address {
  return (relative + base).value() + size;
}
} // namespace mywr::utility
