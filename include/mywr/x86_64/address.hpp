#ifndef MYWR_ADDRESS_HPP_
#define MYWR_ADDRESS_HPP_

namespace mywr {
/**
 * @brief A class for converting pointers to addresses.
 *
 * @details
 * A utility class that converts all pointers to addresses. It is needed
 * primarily for mathematical operations with pointers, which need to be
 * converted into integrals without this class.
 */
class address {
public:
  /**
   * @name Constructors & Destructor.
   * @{
   */

  /**
   * @brief Default constructor.
   *
   * @details
   * `address` can be created by default or be stored as class field.
   *
   * @code{.cpp}
   * // some_src.cpp
   * mywr::address test;
   *
   * // some_header.h
   * class SomeClass {
   * public:
   *    // ...
   * private:
   * mywr::address m_some_addr;
   * };
   *
   * @endcode
   */
  address() = default;
  /**
   * @brief Destructor.
   */
  ~address() = default;

  /**
   * @brief Integral constructor.
   *
   * @details
   * `address` can be created from all integral values (char, bool, uint8/16/32,
   * int, etc...).
   *
   * @code{.cpp}
   * mywr::address addr{0xDEADBEEF};
   * @endcode
   */
  template <class T>
    requires(std::is_integral_v<T>)
  address(T value)
      : m_address(value) {}

  /**
   * @brief Pointer constructor.
   *
   * @details
   * `address` can be created from all pointers. It is mainly used to convert
   * pointers to integrals (a.k.a addresses) to work with mathematical
   * operations.
   *
   * @code{.cpp}
   * const char* str = "Some string!";
   *
   * mywr::address addr{str};
   * @endcode
   */
  template <class T>
  address(T* pointer)
      : m_address(reinterpret_cast<address_t>(pointer)) {}

  /**
   * @}
   */

  /**
   * @name Public Member Functions
   * @{
   */

  /**
   * @brief Returns the address of the passed value.
   *
   * @details
   * It can return both the address of the pointer (if it was passed) and the
   * usual number.
   *
   * @code{.cpp}
   * int value = 24;
   * mywr::address{value}.value() // Returns 24.
   * mywr::address{&value}.value() // Returns address of `value`.
   * @endcode
   *
   * @par Parameters
   *  None.
   */
  MYWR_INLINE const address_t value() const { return m_address; }

  /**
   * @}
   */

  /**
   * @name Operators
   * @{
   */

  /**
   * @brief Returns the address of the passed value.
   * @see address::value()
   */
  MYWR_INLINE operator address_t() const { return m_address; }

  /**
   * @brief Returns the address casted to pointer.
   */
  MYWR_INLINE operator void*() const {
    return reinterpret_cast<void*>(m_address);
  }

  /**
   * @brief Returns the address casted to const pointer.
   */
  MYWR_INLINE operator const void*() const {
    return const_cast<const void*>(reinterpret_cast<void*>(m_address));
  }

  /**
   * @brief Returns an address based on the sum between the other two.
   */
  MYWR_INLINE address operator+(const address& rhs) const {
    return value() + rhs.value();
  }

  /**
   * @brief Returns an address based on the difference between the other two.
   */
  MYWR_INLINE address operator-(const address& rhs) const {
    return value() - rhs.value();
  }

  /**
   * @brief Adds the value of the passed one to the current address.
   */
  MYWR_INLINE void operator+=(const address& rhs) { m_address += rhs.value(); }

  /**
   * @brief Reduces the value of the passed address for the current address.
   */
  MYWR_INLINE void operator-=(const address& rhs) { m_address -= rhs.value(); }

  /**
   * @brief Compares two addresses for equality.
   */
  MYWR_INLINE bool operator==(const address& rhs) const {
    return value() == rhs.value();
  }

  /**
   * @brief Compares two addresses for inequality.
   */
  MYWR_INLINE bool operator!=(const address& rhs) const {
    return value() != rhs.value();
  }
  /**
   * @}
   */

private:
  /**
   * @name Private Member variables
   * @{
   */

  /**
   * @brief The value of the integral or the address of the passed pointer.
   */
  address_t m_address;

  /**
   * @}.
   */
};
} // namespace mywr

#endif
