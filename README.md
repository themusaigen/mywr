# mywr

Cross-platform multiarchitectural (in future) memhacking header-only library written in C++17. The next generation of my [memwrapper](https://github.com/The-Musaigen/memwrapper) project.

## Usage / Examples

```c++
#include <mywr.hpp>

using namespace mywr;

extern void showcase_llmo();

void showcase_llmo() {
  // Define constants for full health points and minimum health points.
  // These values are used to compare and modify the player's health.
  constexpr float kFullHealthPoints = 100.F;
  constexpr float kMinimumHealth    = 10.F;

  // Declare a variable to represent the player's current health.
  // In a real-world scenario, this value would be located in the game's memory.
  float player_health = kMinimumHealth;

  // Simulate a situation where the player's health is being managed by a
  // plugin. In a real-world scenario, this might be a separate DLL or shared
  // library. Here, we're using the llmo library to read and write to the
  // player's health memory. The llmo::read function is used to safely read the
  // player's health from memory. We use a range-based if statement (also known
  // as a structured binding) to declare and initialize
  // a variable 'health' within the scope of the if statement.
  // If the llmo::read function returns a valid value, the value is assigned to
  // 'health'. If the llmo::read function returns an error (i.e., if there was
  // an issue reading the memory), the if statement will not execute the block
  // of code inside the curly braces.
  if (auto health = llmo::read<float>(&player_health)) {
    // Check if the player's health is less than or equal to the minimum health.
    if (*health <= kMinimumHealth) {
      // If the player's health is at or below the minimum health,
      // use the llmo::write function to safely write the full health points to
      // the player's health memory.
      llmo::write<float>(&player_health, kFullHealthPoints);
    }
  }
}

```

For other usage examples, you can look into the `tests` directory or the `demo` directory.

## Platforms / Architectures / Features

| Status | Description                                                             |
| ------ | ----------------------------------------------------------------------- |
| ✅      | Supported and tested.                                                   |
| 💻      | In developing.                                                          |
| ❌      | Unsupported.                                                            |
| ❓      | Maybe supported but untested.                                           |
| ❗      | Not in plans. (Due to lack of technical/physical/material capabilities) |

| Feature                                  | Windows (Int. / Ext.) | Linux (Int. / Ext.) |
| ---------------------------------------- | --------------------- | ------------------- |
| Memory unprotection + RAII               | ✅ / ❌                 | ❌ / ❌               |
| Read/write/copy/fill/compare memory      | ✅ / ❌                 | ❌ / ❌               |
| RAII memory patching                     | ✅ / ❌                 | ❌ / ❌               |
| Signature scanning                       | ✅ / ❌                 | ❌ / ❌               |
| Modules interaction                      | ✅ / ❌                 | ❌ / ❌               |
| Modules enumeration                      | ❌ / ❌                 | ❌ / ❌               |
| Processes enumeration                    | ❌ / ❌                 | ❌ / ❌               |
| Hooking                                  | ❌ / ❌                 | ❌ / ❌               |
| Thread-safety                            | ❌ / ❌                 | ❌ / ❌               |
| Thread-enumeration                       | ❌ / ❌                 | ❌ / ❌               |
| Segment (.text, .data, etc) interaction  | ❌ / ❌                 | ❌ / ❌               |
| Function invoking                        | 💻 / ❌                 | ❌ / ❌               |
| Memory allocation/deallocation           | ❌ / ❌                 | ❌ / ❌               |
| Disassembling (By third-party libraries) | ❌ / ❌                 | ❌ / ❌               |
| Bit utilities                            | ❌ / ❌                 | ❌ / ❌               |
| Virtual table interaction                | ❌ / ❌                 | ❌ / ❌               |

| Platform        | Status |
| --------------- | ------ |
| Windows         | 💻      |
| Linux           | ❌      |
| Linux (FreeBSD) | ❌      |
| Andorid         | ❗      |
| macOS           | ❗      |

| Architecture    | Status |
| --------------- | ------ |
| x86             | 💻      |
| x86_64          | 💻      |
| ARM             | ❗      |
| ARM64 (Aarch64) | ❗      |

## Installation

1. Clone repository (using FetchContent, or just download the repo).
2. Extract `include` directory content and include `mywr.hpp` (or just include `mywr.hpp` if you used FetchContent).

## Third-party libraries

* [google/googletest](https://github.com/google/googletest) (BSD 3-Clause "New" or "Revised" License)
* [TartanLlama/expected](https://github.com/TartanLlama/expected) (CC0-1.0 license)
* [moonpotato/zstring_view](https://github.com/moonpotato/zstring_view) (Apache-2.0 license)

## See also

* [rdbo/libmem](https://github.com/rdbo/libmem)
* [memwrapper](https://github.com/The-Musaigen/memwrapper) (Only for Windows x86)

## License

`mywr` licensed under `MIT` license. See `LICENSE` file to details.
