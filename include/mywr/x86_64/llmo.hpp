/*********************************************************************
 * @file   llmo.hpp
 * @brief  Header containing available low-level memory operations.
 *
 * @author themusaigen
 * @date   September 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_LLMO_HPP_
#define MYWR_LLMO_HPP_

namespace mywr {
/**
 * @brief Namespace contaiting available low-level memory operations.
 */
namespace llmo {
/**
 * @brief Flushes sized memory region.
 *
 * @details
 * In some cases, this function will do nothing. Some processors have an
 * integrated data and instruction cache, in which case the
 * Flush­Instruction­Cache function doesn’t need to do anything. Others such as
 * ARM still have separate instruction and data caches, and in those cases,
 * flushing does real work.
 * ([Source](https://devblogs.microsoft.com/oldnewthing/20190902-00/?p=102828))
 *
 * @param dest    Region to be flushed.
 * @param size    Size of the region to be flushed.
 *
 * @return Success of flushing.
 */
MYWR_FORCEINLINE bool flush(const address& dest, const std::size_t size) {
#if defined(MYWR_WINDOWS)
  return FlushInstructionCache(GetCurrentProcess(), dest, size) != 0;
#elif defined(MYWR_UNIX) && !defined(MYWR_FEATURE_NO_FLUSH_CACHE)
  return cacheflush(target, size, ICACHE) == 0;
#else
  return true;
#endif
}

/**
 * @brief Reads data at the memory address and converts it to the specified
 * type.
 *
 * @code{.cpp}
 * uint32_t player_health = mywr::llmo::read<uint32_t>(0xDEADBEEF);
 * @endcode
 *
 * @tparam T The type of data to be read.
 *
 * @param[in] dest The address of the memory where the data is stored.
 *
 * @return Data.
 */
template <typename T>
MYWR_FORCEINLINE T read(const address& dest) {
  // Unprotect memory region.
  protect::scoped_protect protect(dest,
                                  sizeof(T),
                                  protect::memory_prot::kExecuteReadWrite);
  // And read data.
  return *reinterpret_cast<T*>(dest.value());
}

/**
 * @brief Writes data to an address in memory.
 *
 * @code{.cpp}
 * mywr::llmo::write<uint32_t>(0xDEADBEEF, 100);
 * @endcode
 *
 * @tparam T The type of data to be written.
 *
 * @param[in] dest  The address in memory where the data needs to be written.
 * @param[in] value The data that needs to be written.
 */
template <typename T>
MYWR_FORCEINLINE void write(const address& dest, T value) {
  // Unprotect memory region.
  protect::scoped_protect protect(dest,
                                  sizeof(T),
                                  protect::memory_prot::kExecuteReadWrite);

  // Write data.
  *reinterpret_cast<T*>(dest.value()) = value;

  // Flush the CPU`s cache.
  flush(dest, sizeof(T));
}

/**
 * @brief Copies bytes from a source memory area to a destination memory area,
 * where both areas may not overlap.
 *
 * @code{.cpp}
 * mywr::llmo::copy(0xDEADBEEF, '\xC3', 1);
 * @endcode
 *
 * @param[in]  dest The memory area to copy to.
 * @param[in]  src  The memory area to copy from.
 * @param[in]  size The number of bytes to copy.
 */
MYWR_FORCEINLINE void
copy(const address& dest, const address& src, const std::size_t size) {
  // Unprotect memory region.
  protect::scoped_protect protect(dest,
                                  size,
                                  protect::memory_prot::kExecuteReadWrite);
  // Process `memcpy`.
  ::memcpy(dest, src, size);
  // And flush CPU`s cache.
  flush(dest, size);
}

/**
 * @brief Fills a value to a destination memory area with specified size.
 *
 * @code{.cpp}
 * mywr::llmo::fill(0xDEADBEEF, 0xC3, 1);
 * @endcode
 *
 * @param[in]  dest   The memory area to copy to.
 * @param[in]  value  The value to fill in.
 * @param[in]  size   The number of bytes to fill.
 */
MYWR_FORCEINLINE void
fill(const address& dest, const int value, const std::size_t size) {
  // Unprotect memory region.
  protect::scoped_protect protect(dest,
                                  size,
                                  protect::memory_prot::kExecuteReadWrite);

  // Process `memset`.
  ::memset(dest, value, size);

  // Flush CPU`s cache.
  flush(dest, size);
}

/**
 * @brief Compares 2 memory areas.
 *
 * @code{.cpp}
 * int value = 24;
 * int value2 = 24;
 *
 * int result = mywr::llmo::compare(&value, &value2, sizeof(int));
 * @endcode
 *
 * @param[in] buf0 The first buffer to compare.
 * @param[in] buf1 The second buffer to compare.
 * @param[in] size The number of bytes to compare.
 *
 * @retval <0 buffer1 less than buffer2.
 * @retval 0  buffer1 identical to buffer2.
 * @retval >0 buffer1 greater than buffer2.
 */
MYWR_FORCEINLINE int
compare(const address& buf0, const address& buf1, const std::size_t size) {
  // Unprotect the buffers.
  protect::scoped_protect protect0(buf0,
                                   size,
                                   protect::memory_prot::kExecuteReadWrite);
  protect::scoped_protect protect1(buf1,
                                   size,
                                   protect::memory_prot::kExecuteReadWrite);

  // Compare.
  return ::memcmp(buf0, buf1, size);
}
} // namespace llmo
} // namespace mywr

#endif // !MYWR_LLMO_HPP_
