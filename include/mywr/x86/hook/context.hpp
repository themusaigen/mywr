/*********************************************************************
 * @file   context.hpp
 * @brief  Module containing CPU context in x86.
 * 
 * @author themusaigen
 * @date   October 2024
 * @copyright MIT License.
 *********************************************************************/
#ifndef MYWR_CONTEXT32_HPP_
#define MYWR_CONTEXT32_HPP_

#define MYWR_REGX(ch)                                                          \
  union {                                                                      \
    std::uint32_t e##ch##x{};                                                  \
    struct {                                                                   \
      union {                                                                  \
        std::uint16_t ch##x;                                                   \
        struct {                                                               \
          std::uint8_t ch##l;                                                  \
          std::uint8_t ch##h;                                                  \
        };                                                                     \
      };                                                                       \
    };                                                                         \
  };

#define MYWR_REG(ch)                                                           \
  union {                                                                      \
    std::uint32_t e##ch{};                                                     \
    union {                                                                    \
      std::uint16_t ch;                                                        \
      std::uint8_t  ch##l;                                                     \
    };                                                                         \
  };

namespace mywr {
namespace hook {
namespace impl {

struct context {
  MYWR_REGX(a)
  MYWR_REGX(b)
  MYWR_REGX(c)
  MYWR_REGX(d)
  MYWR_REG(sp)
  MYWR_REG(bp)
  MYWR_REG(si)
  MYWR_REG(di)

  address_t eip{};
};
} // namespace impl
} // namespace hook
} // namespace mywr

#undef MYWR_REGX
#undef MYWR_REG
#undef MYWR_RESERVED
#undef MYWR_FLAG
#undef MYWR_BIG_FLAG

#endif // !MYWR_CONTEXT32_HPP_
