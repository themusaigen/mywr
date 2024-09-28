/*********************************************************************
 * @file   protect.hpp
 * @brief  Header with tools for interacting with memory`s protection.
 *
 * @author themusaigen
 * @date   September 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_PROTECT_HPP_
#define MYWR_PROTECT_HPP_

namespace mywr {
/**
 * @brief Namespace containing tools for interacting with memory`s protection.
 */
namespace protect {
/**
 * @class memory_prot
 * @brief Class that contains all possible memory`s protection constants for
 * Windows and Linux.
 */

/**
 * @enum memory_prot::Enum
 * @brief Enum that contains all possible memory`s protection constants for
 * Windows and Linux.
 */

/**
 * @var memory_prot::Enum memory_prot::kUnknown
 * @brief Unknown memory protection. It can be interpreted as an error.
 */

/**
 * @var memory_prot::Enum memory_prot::kNoAccess
 * @brief There is no access to this memory section. It is not possible to
 * execute/read/write.
 */

/**
 * @var memory_prot::Enum memory_prot::kReadOnly
 * @brief A read-only memory section. Windows only.
 */

/**
 * @var memory_prot::Enum memory_prot::kWriteCopy
 * @brief Copy-on-write access to a mapped view of a file mapping object. An
 * attempt to write to a committed copy-on-write page results in a private copy
 * of the page being made for the process. Windows only.
 */

/**
 * @var memory_prot::Enum memory_prot::kExecute
 * @brief The memory section intended for execution.
 */

/**
 * @var memory_prot::Enum memory_prot::kRead
 * @brief The memory section intended for reading.
 */

/**
 * @var memory_prot::Enum memory_prot::kWrite
 * @brief The memory section intended for writing.
 */

/**
 * @var memory_prot::Enum memory_prot::kReadWrite
 * @brief A set of memory flags that is intended for writing and reading.
 */

/**
 * @var memory_prot::Enum memory_prot::kExecuteRead
 * @brief set of memory flags that is intended for executing and reading.
 */

/**
 * @var memory_prot::Enum memory_prot::kExecuteReadWrite
 * @brief A set of memory flags that is intended for executing, reading and
 * writing.
 */

/**
 * @var memory_prot::Enum memory_prot::kExecuteWriteCopy
 * @brief A set of memory flags that is intended for executing, reading and
 * copy-on-writing. Windows only
 */
class memory_prot {
public:
  enum Enum : std::uint32_t {
    kUnknown = 0,
    kNoAccess = (1 << 0),
    kReadOnly = (1 << 1),
    kWriteCopy = (1 << 2),
    kExecute = (1 << 3),
    kRead = (1 << 4),
    kWrite = (1 << 5),

    kReadWrite = kRead | kWrite,
    kExecuteRead = kRead | kExecute,
    kExecuteReadWrite = kExecute | kRead | kWrite,
    kExecuteWriteCopy = kExecute | kRead | kWriteCopy
  };
};

/**
 * @brief Converts from OS-specific constant to `memwrapper` constant.
 *
 * @code{.cpp}
 * // Windows
 * auto execute_constant = mywr::protect::to_protection_constant(PAGE_EXECUTE);
 * @endcode
 *
 * @param[in] protect OS-specific constant to be converted.
 *
 * @return `memwrapper` memory protection constant.
 */
static memory_prot::Enum to_protection_constant(const std::uint32_t protect) {
#if defined(MYWR_WINDOWS)
  switch (protect) {
  case PAGE_NOACCESS:
    return memory_prot::kNoAccess;
  case PAGE_READONLY:
    return memory_prot::kReadOnly;
  case PAGE_READWRITE:
    return memory_prot::kReadWrite;
  case PAGE_WRITECOPY:
    return memory_prot::kWriteCopy;
  case PAGE_EXECUTE:
    return memory_prot::kExecute;
  case PAGE_EXECUTE_READ:
    return memory_prot::kExecuteRead;
  case PAGE_EXECUTE_READWRITE:
    return memory_prot::kExecuteReadWrite;
  case PAGE_EXECUTE_WRITECOPY:
    return memory_prot::kExecuteWriteCopy;
  default:
    return memory_prot::kUnknown;
  }
#elif defined(MYWR_UNIX) && !defined(MYWR_FEATURE_NO_MPROTECT)
  switch (protect) {
  case PROT_NONE:
    return memory_prot::kNoAccess;
  case PROT_READ:
    return memory_prot::kRead;
  case PROT_WRITE:
    return memory_prot::kWrite;
  case PROT_EXEC:
    return memory_prot::kExecute;
  case PROT_READ | PROT_WRITE:
    return memory_prot::kReadWrite;
  case PROT_READ | PROT_EXEC:
    return memory_prot::kExecuteRead;
  case PROT_READ | PROT_WRITE | PROT_EXEC:
    return memory_prot::kExecuteReadWrite;
  default:
    return memory_prot::kUnknown;
  }
#else
  return memory_prot::kRead | memory_prot::kWrite | memory_prot::kExecute;
#endif
}

/**
 * @brief Converts from memwrapper`s constant to OS-specific protection
 * constant.
 *
 * @code{.cpp}
 * // Windows
 * using mywr::protect::memory_prot;
 *
 * // PAGE_EXECUTE
 * auto execute_constant =
 * mywr::protect::from_protection_constant(memory_prot::kExecute);
 * @endcode
 *
 * @param[in] protect `memwrapper` specific protection constant.
 *
 * @return OS-specific protection constant.
 */
static uint32_t from_protection_constant(const memory_prot::Enum protect) {
#if defined(MYWR_WINDOWS)
  switch (protect) {
  case memory_prot::kNoAccess:
    return PAGE_NOACCESS;
  case memory_prot::kReadOnly:
    return PAGE_READONLY;
  case memory_prot::kReadWrite:
    return PAGE_READWRITE;
  case memory_prot::kWriteCopy:
    return PAGE_WRITECOPY;
  case memory_prot::kExecute:
    return PAGE_EXECUTE;
  case memory_prot::kExecuteRead:
    return PAGE_EXECUTE_READ;
  case memory_prot::kExecuteReadWrite:
    return PAGE_EXECUTE_READWRITE;
  case memory_prot::kExecuteWriteCopy:
    return PAGE_EXECUTE_WRITECOPY;
  default:
    return 0;
  }
#elif defined(MYWR_UNIX) && !defined(MYWR_FEATURE_NO_MPROTECT)
  switch (protect) {
  case memory_prot::kNoAccess:
    return PROT_NONE;
  case memory_prot::kReadOnly:
    return PROT_READ;
  case memory_prot::kRead:
    return PROT_READ;
  case memory_prot::kWrite:
    return PROT_WRITE;
  case memory_prot::kExecute:
    return PROT_EXEC;
  case memory_prot::kReadWrite:
    return PROT_READ | PROT_WRITE;
  case memory_prot::kExecuteReadWrite:
    return PROT_READ | PROT_WRITE | PROT_EXEC;
  case memory_prot::kExecuteRead:
    return PROT_READ | PROT_EXEC;
  default:
    return PROT_NONE;
  }
#else
  return 0;
#endif
}

/**
 * @brief Returns current protection flags of specified memory address.
 *
 * @details
 * On Windows uses `VirtualQuery` to get protection flags of memory area. On
 * Linux parses `proc/self/maps`. Returns @ref memory_prot::kNoAccess if error
 * acquired.
 *
 * @code{.cpp}
 * auto protect = mywr::protect::get_protect(0xDEADBEEF);
 * @endcode
 *
 * @param[in] target Area where must to get protection flags.
 *
 * @return `memwrapper` specific memory protection constant.
 */
static memory_prot::Enum get_protect(const address& target) {
#if defined(MYWR_WINDOWS)
  MEMORY_BASIC_INFORMATION mbi{};
  if (!VirtualQuery(target, &mbi, sizeof(mbi)))
    return memory_prot::kUnknown;

  return to_protection_constant(mbi.Protect);
#elif defined(MYWR_UNIX) && !defined(MYWR_FEATURE_NO_MPROTECT)
  // Source:
  // https://github.com/kin4stat/kthook/blob/main/include/kthook/x86_64/kthook_x86_64_detail.hpp#L591
  struct map_info {
    std::uintptr_t start;
    std::uintptr_t end;

    unsigned prot;
  };

  std::vector<map_info> result;

  #ifdef __FreeBSD__
  std::ifstream proc_maps{"/proc/curproc/map"};
  #else
  std::ifstream proc_maps{"/proc/self/maps"};
  #endif
  std::string line;

  while (std::getline(proc_maps, line)) {
    map_info parse_result{};

    auto start = 0u;
    auto i = 0u;
    while (line[i] != '-') {
      ++i;
    }

    std::from_chars(&line[start], &line[i], parse_result.start, 16);

    start = ++i;
    while (line[i] != '\t' && line[i] != ' ') {
      ++i;
    }
    std::from_chars(&line[start], &line[i], parse_result.end, 16);

  #ifdef __FreeBSD__
    while (line[i] != '\t' && line[i] != ' ') {
      ++i;
    }
    ++i; // skip
    while (line[i] != '\t' && line[i] != ' ') {
      ++i;
    }
    ++i; // skip
    while (line[i] != '\t' && line[i] != ' ') {
      ++i;
    }
    ++i; // skip
  #endif

    start = ++i;
    while (line[i] != '\t' && line[i] != ' ') {
      ++i;
    }

    if (line[start++] == 'r')
      parse_result.prot |= PROT_READ;
    if (line[start++] == 'w')
      parse_result.prot |= PROT_WRITE;
    if (line[start++] == 'x')
      parse_result.prot |= PROT_EXEC;
    result.push_back(parse_result);
  }

  address_t address = target.value();
  for (auto& mi : result) {
    if (address >= mi.start && address <= mi.end) {
      return to_protection_constant(mi.prot);
    }
  }

  return memory_prot::kUnknown;
#else
  return memory_prot::kUnknown;
#endif
}

/**
 * @brief Sets new protection of specified memory address.
 *
 * @details
 * On Windows uses `VirtualProtect`. On Linux uses `mprotect`.
 *
 * @code{.cpp}
 * using mywr::protect::memory_prot;
 *
 * auto prev_protect = mywr::protect::set_protect(0xDEADBEEF, 4096,
 * memory_prot::kExecuteReadWrite);
 * @endcode
 */
static memory_prot::Enum set_protect(const address& target,
                                     const std::size_t size,
                                     const memory_prot::Enum protect) {
#if defined(MYWR_WINDOWS)
  DWORD old_protect{};
  if (!VirtualProtect(target,
                      size,
                      from_protection_constant(protect),
                      &old_protect))
    return memory_prot::kUnknown;

  return to_protection_constant(old_protect);
#elif defined(MYWR_UNIX) && !defined(MYWR_FEATURE_NO_MPROTECT)
  address_t address = target.value();
  address_t aligned_address = address & ~(sysconf(_SC_PAGE_SIZE) - 1u);
  size_t aligned_size = size + (address - aligned_address);

  // Retrieve old protect on UNIX systems.
  memory_prot::Enum old_protect = get_protect(address);

  // Set new protect.
  if (mprotect(reinterpret_cast<void*>(aligned_address),
               aligned_size,
               from_protection_constant(protect)) != 0)
    return memory_prot::kUnknown;

  // And return old protect.
  return old_protect;
#else
  return memory_prot::kUnknown;
#endif
}

/**
 * @brief RAII class for protection.
 *
 * @details
 * During construction, it sets the specified memory protection and saves the
 * old one. When exiting the scope, it restores the old memory protection, if no
 * errors have occurred.
 */
class scoped_protect {
public:
  /**
   * @name Constructors & Destructor.
   * @{
   */

  /**
   * @brief Default constructor forbidden.
   */
  scoped_protect() = delete;

  /**
   * @brief Copy constructor forbidden.
   */
  scoped_protect(const scoped_protect&) = delete;

  /**
   * @brief Move constructor forbidden.
   */
  scoped_protect(scoped_protect&&) = delete;

  /**
   * @name Operators
   * @{
   */

  /**
   * @brief Copy operator forbidden.
   */
  void operator=(const scoped_protect&) = delete;

  /**
   * @brief Move operator forbidden.
   */
  void operator=(scoped_protect&&) = delete;

  /**
   * @}
   */

  /**
   * @brief Main constructor.
   *
   * @details
   * Saves all passed parameters and sets new protection.
   *
   * @param[in] target  The memory area where you want to install a new
   * protection.
   * @param[in] size    The size of the area to install a new protection.
   * @param[in] protect A new memory protection that will be installed to this
   * memory area.
   */
  scoped_protect(const address& target,
                 const std::size_t size,
                 const memory_prot::Enum protect)
      : m_target(target)
      , m_size(size)
      , m_old_protect(set_protect(target, size, protect)) {}

  /**
   * @brief Destructor.
   *
   * @details
   * It will restore memory protection only if there were no errors.
   */
  ~scoped_protect() {
    if (good())
      set_protect(m_target, m_size, m_old_protect);
  }
  /**
   * @}
   */

  /**
   * @name Public Member Functions
   * @{
   */

  /**
   * @brief Returns the result of the memory protection installation.
   *
   * @details
   * `true` means "no errors", `false` an error occurred.
   */
  MYWR_INLINE bool good() const {
    return m_old_protect != memory_prot::kUnknown;
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
   * @brief The memory area where you want to install a new protection.
   */
  address m_target{0};
  /**
   * @brief The size of the area to install a new protection.
   */
  size_t m_size{0};
  /**
   * @brief A old memory protection that will be installed to this memory area
   * during destruction.
   */
  memory_prot::Enum m_old_protect{};

  /**
   * @}
   */
};
} // namespace protect
} // namespace mywr

#endif // !MYWR_PROTECT_HPP_
