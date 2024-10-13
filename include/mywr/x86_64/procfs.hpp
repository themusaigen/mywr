/*********************************************************************
 * @file   procfs.hpp
 * @brief  Module containing UNIX `proc` filesystem parsers.
 *
 * @author themusaigen
 * @date   October 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_PROCFS_HPP_
#define MYWR_PROCFS_HPP_

#if defined(MYWR_FEATURE_PROCFS_PARSER_DUMP)
  #include <iostream>
#endif

namespace mywr {
/**
 * @brief Namespace containing UNIX procfs parsers.
 */
namespace procfs {
/**
 * @class memory_region
 * @brief Data-structure for each row in /proc/self/maps.
 */

/**
 * @enum memory_region::path_type
 * @brief Enum containing all possible supported types of data stored in
 * pathname.
 */

/**
 * @var memory_region::path_type memory_region::kUnknown
 * @brief Unknown path type.
 */

/**
 * @var memory_region::path_type memory_region::kNone
 * @brief Anonymous memory region.
 */

/**
 * @var memory_region::path_type memory_region::kFile
 * @brief Memory region allocated from a file.
 */

/**
 * @var memory_region::path_type memory_region::kHeap
 * @brief Memory region allocated on heap.
 */

/**
 * @var memory_region::path_type memory_region::kStack
 * @brief Memory region allocated on stack.
 */

/**
 * @var memory_region::path_type memory_region::kVvar
 * @brief Undocumented pathtype, maybe virtual var (?). Ethernet mentions .vvar
 * special section in the kernel binary image.
 * (https://lwn.net/Articles/615809/)
 */

/**
 * @var memory_region::path_type memory_region::kVdso
 * @brief Stands for virtual dynamic shared object.
 */

/**
 * @var memory_region::path_type memory_region::kThreadstack
 * @brief Memory region allocated on the stack for specified thread
 */

/**
 * @var memory_region::path_type memory_region::kAnon
 * @brief Also anonymous memory region, but the some name specified.
 */

/**
 * @var memory_region::path_type memory_region::kAnonShmem
 * @brief Also anonymous shared memory region, but the some name specified.
 */
struct memory_region {
  enum path_type : std::uint32_t {
    kUnknown,
    kNone,
    kFile,
    kVvar,
    kVdso,
    kStack,
    kThreadstack,
    kHeap,
    kAnon,
    kAnonShmem
  };

  /**
   * @brief The begin of the memory region.
   */
  std::uintptr_t begin{};

  /**
   * @brief The end of the memory region.
   */
  std::uintptr_t end{};

  /**
   * @brief Permissions of this regions (can write, can read, can execute).
   */
  std::uint32_t permissions{};

  /**
   * @brief Is this memory region shared with other processes?
   */
  bool is_shared{};

  /**
   * @brief Is this memory region private and copy-on-write?
   */
  bool is_private{};

  /**
   * @brief If the region was mapped from a file (using mmap), this is the
   * offset in the file where the mapping begins. If the memory was not mapped
   * from a file, it's just 0..
   */
  std::size_t offset{};

  /**
   * @brief If the region was mapped from a file, this is the major
   * device number (in hex) where the file lives.
   */
  std::uint32_t dev_major{};

  /**
   * @brief If the region was mapped from a file, this is the minor
   * device number (in hex) where the file lives.
   */
  std::uint32_t dev_minor{};

  /**
   * @brief If the region was mapped from a file, this is the file number.
   */
  unsigned long long inode{};

  /**
   * @brief If the region was mapped from a file, this is the name of the file.
   * This field is blank for anonymous mapped regions.
   */
  std::string pathname{};

  /**
   * @brief There are also special regions with names like [heap], [stack],
   * [vdso], and other. [vdso] stands for virtual dynamic shared object. It's
   * used by system calls to switch to kernel mode.
   */
  path_type pathtype{kUnknown};
};

/**
 * @brief A simple class for writing your own procfs file parsers.
 */
class parser {
public:
  /**
   * @brief Constructor on path. Opens the file on the passed path and reads
   * first line.
   */
  parser(std::string_view path) {
    /**
     * Open the file.
     */
    m_file.open(path.data());

    /**
     * Read first line.
     */
    next_line();
  }

  /**
   * @brief Indicates whether the end of the file has been reached.
   * @return True if file ended or fail opening failed.
   */
  bool eof() const { return m_file.eof() || fail(); }

  /**
   * @brief Indicates whether the end of the current line has been reached.
   */
  bool eol() const { return m_cursor >= m_line.size(); }

  /**
   * @brief Indicates wherther the file opening failed.
   */
  bool fail() const { return m_file.fail(); }

  /**
   * @brief Announces a new capture area. It is necessary to capture numbers or
   * strings.
   *
   * @code{.cpp}
   * parser.next_until_space();
   * parser.scope();
   * parser.next_until(':');
   * parser.grab_number(my_struct.some_hex_number, 16);
   * @endcode
   */
  void scope() { m_scope = m_cursor; }

  /**
   * @brief Moves the reading cursor until it encounters the desired character.
   *
   * @param[in] ch Char to wait.
   */
  void next_until(char ch) {
    while ((now() != ch) && !eol())
      ++m_cursor;
  }

  /**
   * @brief Moves the reading cursor until it encounters the tabulation or
   * space.
   */
  void next_until_space() {
    while ((now() != '\t' && now() != ' ') && !eol())
      ++m_cursor;
  }

  /**
   * @brief Moves the reading cursor until it encounters any character.
   */
  void next_until_any_char() {
    while ((now() == '\t' || now() == ' ') && !eol())
      ++m_cursor;
  }

  /**
   * @brief Moves the reading cursor by a certain number of characters.
   */
  void next_for(std::size_t count) {
    m_cursor = std::min(m_cursor + count, m_line.size());
  }

  /**
   * @brief Moves the reading cursor to end of the line.
   */
  void next_to_eol() { m_cursor = m_line.size(); }

  /**
   * @brief Converts the characters received through the capture area into a
   * number with a certain radix.
   *
   * @tparam T The type of variable to write the capture result to. It can only
   * be integral.
   *
   * @param[in] value The variable to write the result to.
   * @param[in] radix The system of calculation of number.
   */
  template <typename T,
            typename = std::enable_if_t<
                std::is_integral_v<T> || std::is_floating_point_v<T> ||
                    std::is_same_v<T, double> || std::is_same_v<T, long double>,
                T>>
  void grab_number(T& value, int radix = 10) {
    std::from_chars(&m_line[m_scope], &m_line[m_cursor], value, radix);
  }

  /**
   * @brief Returns a string of characters received from the capture area.
   */
  std::string grab_string() {
    return m_line.substr(m_scope, m_cursor - m_scope);
  }

  /**
   * @brief Returns the current character without moving the reading cursor.
   */
  char now() const { return m_line[m_cursor]; }

  /**
   * @brief Returns the current character, and then moves the reading cursor..
   */
  char next() { return m_line[m_cursor++]; }

  /**
   * @brief Moves to the next line.
   */
  void next_line() {
    if (!fail())
      std::getline(m_file, m_line);

    m_scope = 0;
    m_cursor = 0;
  }

#if defined(MYWR_FEATURE_PROCFS_PARSER_DUMP)
  /**
   * @brief Outputs a line starting from the position of the reading cursor to
   * the end of the line.
   */
  void dump_from_cursor() {
    std::cout << m_line.substr(m_cursor, m_line.size() - m_cursor) << '\n';
  }

  /**
   * @brief Outputs whole line.
   */
  void dump_line() { std::cout << m_line << '\n'; }
#endif

private:
  /**
   * @brief Current file we parsing.
   */
  std::ifstream m_file{};

  /**
   * @brief Current line we parsing.
   */
  std::string m_line{};

  /**
   * @brief Capture area start.
   */
  std::size_t m_scope{};

  /**
   * @brief Reading cursor position.
   */
  std::size_t m_cursor{};
};

/**
 * @brief Parses the /proc/self/maps file to get information about mapped memory
 * regions.
 *
 * @param[out] regions A dynamic array of regions where the parser will add new
 * entries.
 */
static void parse_maps(std::vector<memory_region>& regions) {
#if defined(MYWR_UNIX)
  #if defined(__FreeBSD__)
  constexpr auto kProcMapsPath = "/proc/curproc/map";
  #else
  constexpr auto kProcMapsPath = "/proc/self/maps";
  #endif
  constexpr auto kVdso = "[vdso]";
  constexpr auto kVvar = "[vvar]";
  constexpr auto kStack = "[stack]";
  constexpr auto kThreadstack = "[stack:";
  constexpr auto kAnon = "[anon:";
  constexpr auto kAnonShmem = "[anon_shmem:";
  constexpr auto kHeap = "[heap]";

  parser parser{kProcMapsPath};

  while (!parser.eof()) {
    memory_region region{};

    /**
     * Handles pages begin and end.
     */
    parser.scope();
    parser.next_until('-');
    parser.grab_number(region.begin, 16);
    parser.next();

    parser.scope();
    parser.next_until_space();
    parser.grab_number(region.end, 16);
    parser.next();

    /**
     * Handle permissions.
     */
    if (parser.next() == 'r')
      region.permissions |= PROT_READ;

    if (parser.next() == 'w')
      region.permissions |= PROT_WRITE;

    if (parser.next() == 'x')
      region.permissions |= PROT_EXEC;

    /**
     * Handle private or shared mapping.
     */
    char ps = parser.next();
    if (ps == 'p')
      region.is_private = true;
    else if (ps == 's')
      region.is_shared = true;

    /**
     * Shift on the start of `offset`.
     */
    parser.next_until_space();
    parser.next();

    /**
     * Handle offset.
     */
    parser.scope();
    parser.next_until_space();
    parser.grab_number(region.offset, 16);

    /**
     * Shift on the start of `dev`.
     */
    parser.next_until_space();
    parser.next();

    /**
     * Handle dev major and minor.
     */
    parser.scope();
    parser.next_until(':');
    parser.grab_number(region.dev_major);
    parser.next();

    parser.scope();
    parser.next_until_space();
    parser.grab_number(region.dev_minor);
    parser.next();

    /**
     * Handle inode.
     */
    parser.scope();
    parser.next_until_space();
    parser.grab_number(region.inode);

    /**
     * Handle pathname.
     */
    parser.next_until_any_char();
    parser.scope();
    parser.next_to_eol();

    region.pathname = std::move(parser.grab_string());

  #if defined(MYWR_FEATURE_PROCFS_PATHTYPE_DEDUCTION)
    if (region.pathname == kStack)
      region.pathtype = memory_region::kStack;
    else if (region.pathname == kHeap)
      region.pathtype = memory_region::kHeap;
    else if (region.pathname == kVvar)
      region.pathtype = memory_region::kVvar;
    else if (region.pathname == kVdso)
      region.pathtype = memory_region::kVdso;
    else if (!region.pathname.empty())
      region.pathtype = memory_region::kFile;
    else if (region.pathname.find(kThreadstack) != std::string::npos)
      region.pathtype = memory_region::kThreadstack;
    else if (region.pathname.find(kAnon) != std::string::npos)
      region.pathtype = memory_region::kAnon;
    else if (region.pathname.find(kAnonShmem) != std::string::npos)
      region.pathtype = memory_region::kAnonShmem;
    else
      region.pathtype = memory_region::kNone;
  #endif

    regions.push_back(std::move(region));

    parser.next_line();
  }
#endif
}
} // namespace procfs
} // namespace mywr

#endif // !MYWR_PROCFS_HPP_
