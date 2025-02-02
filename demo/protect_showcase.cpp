#include <iostream>

extern void showcase_protect();

// In most cases, you will not use functions from the `protect` namespace. They
// are needed to implement the internal functionality of the library. So if you
// really need examples, it's worth a look "tests/internal/protect.cpp".

void showcase_protect() {
  std::cout << "showcase_protect(): i am do nothing, see "
               "\"tests/internal/protect.cpp\""
            << '\n';
}
