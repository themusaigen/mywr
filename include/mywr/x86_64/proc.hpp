/*********************************************************************
 * @file   proc.hpp
 * @brief  Module containing UNIX `proc` filesystem parsers.
 *
 * @author themusaigen
 * @date   October 2024
 * @copyright MIT License.
 *********************************************************************/
#if defined(MYWR_UNIX)
  #ifndef MYWR_PROC_HPP_
    #define MYWR_PROC_HPP_

    #if defined(MYWR_FEATURE_PROC_PARSER_DUMP)
      #include <iostream>
    #endif

namespace mywr {
namespace proc {
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

  std::uintptr_t begin{};
  std::uintptr_t end{};
  std::uint32_t permissions{};
  bool is_shared{};
  bool is_private{};
  std::size_t offset{};
  std::uint32_t dev_major{};
  std::uint32_t dev_minor{};
  unsigned long long inode{};
  std::string pathname{};
  path_type pathtype{kUnknown};
};

class parser {
public:
  parser(std::string_view path) {
    m_file.open(path.data());
    next_line();
  }

  bool eof() const { return m_file.eof(); }
  bool eol() const { return m_cursor >= m_line.size(); }

  void scope() { m_scope = m_cursor; }

  void next_until(char ch) {
    while ((now() != ch) && !eol())
      ++m_cursor;
  }

  void next_until_space() {
    while ((now() != '\t' && now() != ' ') && !eol())
      ++m_cursor;
  }

  void next_until_any_char() {
    while ((now() == '\t' || now() == ' ') && !eol())
      ++m_cursor;
  }

  void next_to_eol() { m_cursor = m_line.size(); }

  template <typename T, typename = std::enable_if_t<std::is_integral_v<T>, T>>
  void grab_number(T& value, int radix = 10) {
    std::from_chars(&m_line[m_scope], &m_line[m_cursor], value, radix);
  }

  std::string grab_string() {
    return m_line.substr(m_scope, m_cursor - m_scope);
  }

  char now() const { return m_line[m_cursor]; }
  char next() { return m_line[m_cursor++]; }

  void next_line() {
    std::getline(m_file, m_line);
    m_scope = 0;
    m_cursor = 0;
  }

    #if defined(MYWR_FEATURE_PROC_PARSER_DUMP)
  void dump_from_cursor() {
    std::cout << m_line.substr(m_cursor, m_line.size() - m_cursor) << '\n';
  }

  void dump_line() { std::cout << m_line << '\n'; }
    #endif

private:
  std::ifstream m_file{};
  std::string m_line{};
  std::size_t m_scope{};
  std::size_t m_cursor{};
};

static void parse_maps(std::vector<memory_region>& regions) {
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

    #if defined(MYWR_FEATURE_PROC_PATHTYPE_DEDUCTION)
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
}
} // namespace proc
} // namespace mywr

  #endif // !MYWR_PROC_HPP_
#endif
