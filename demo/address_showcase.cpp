#include <mywr.hpp>

extern void showcase_address();

// For example, you are writing a function that produces something with the
// memory address passed to it. You need to be able to pass both pointers and
// addresses to some part of the code in the program. Normally, you have two
// options.: 1) write an overload with a template parameter. 2) overload the
// function for some types, for example (uint32_t, uint64_t, void*). However, if
// you have a mywr::address class, you don't have to worry about it anymore.
namespace {
void do_something_with_memory(const mywr::address& /*address*/,
                              std::size_t /*size*/) {
  // `valid` means it not a nullptr.
  // if (address.valid()) {
  //  do something.
  // }
}
} // namespace

void showcase_address() {
  constexpr auto PlayerNicknameBuffer        = 0x1337;
  constexpr auto PlayerNicknameBufferMaxSize = 24;

  // This function is called via an address that points to the character buffer
  // of the player's nickname.
  do_something_with_memory(PlayerNicknameBuffer, PlayerNicknameBufferMaxSize);

  // NOLINTBEGIN(*-no-int-to-ptr)
  char* nick = *reinterpret_cast<char**>(PlayerNicknameBuffer);
  // NOLINTEND(*-no-int-to-ptr)

  // However, this function is called via a pointer to the player's nickname
  // buffer. The 'address` class will "swallow" this without problems, and the
  // `do_something_with_memory` function does not need to worry about what is
  // passed to it.
  do_something_with_memory(nick, PlayerNicknameBufferMaxSize);
}
