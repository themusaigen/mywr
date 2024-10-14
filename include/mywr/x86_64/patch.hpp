/*********************************************************************
 * @file   patch.hpp
 * @brief  Module containing RAII LLMO patches.
 *
 * @author themusaigen
 * @date   October 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_PATCH_HPP_
#define MYWR_PATCH_HPP_

namespace mywr {
namespace patch {
template <typename T>
class scoped_write {
public:
  scoped_write() = default;
  scoped_write(const scoped_write&) = delete;
  scoped_write(scoped_write&& rhs) { operator=(std::move(rhs)); }

  scoped_write(const address& dest, T value) {
    retarget(dest);
    install(value);
  }

  ~scoped_write() {
    remove();
  }

  bool install(T value) {
    if (m_installed)
      return false;

    if (!m_dest)
      return false;

    m_backup = llmo::read<T>(m_dest);

    llmo::write<T>(m_dest, value);

    m_installed = true;
    return true;
  }

  bool remove() {
    if (!m_installed)
      return false;

    if (!m_dest)
      return false;

    llmo::write<T>(m_dest, m_backup);

    m_installed = false;
    return true;
  }

  bool installed() const { return m_installed; }

  void retarget(const address& dest) {
    if (m_installed)
      remove();

    m_dest = dest.value();
  }

  void operator=(const scoped_write&) = delete;
  void operator=(scoped_write&& rhs) {
    std::swap(m_dest, rhs.m_dest);
    std::swap(m_backup, rhs.m_backup);
    std::swap(m_installed, rhs.m_installed);

    /**
     * Nullify rhs dest to prevent backuping.
     */
    rhs.m_dest = 0;
  }

private:
  address_t m_dest{};
  T m_backup{};

  bool m_installed{};
};
} // namespace patch
} // namespace mywr

#endif // !MYWR_PATCH_HPP_
