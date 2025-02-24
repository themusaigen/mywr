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


| Feature                                                      | Windows (I/E) | Linux (I/E) | Description                                                     |
| ------------------------------------------------------------ | ------------- | ----------- | --------------------------------------------------------------- |
| **1. Memory Operations**                                     |
| &nbsp;&nbsp;&nbsp;&nbsp;1.1 Read/Write/Copy/Fill/Compare     | ✅/❌           | ❌/❌         | Basic memory manipulation operations                            |
| &nbsp;&nbsp;&nbsp;&nbsp;1.2 Unprotection + RAII              | ✅/❌           | ❌/❌         | Temporarily change memory protection with automatic restoration |
| &nbsp;&nbsp;&nbsp;&nbsp;1.3 RAII Patching                    | ✅/❌           | ❌/❌         | Automatically revert memory changes when going out of scope     |
| &nbsp;&nbsp;&nbsp;&nbsp;1.4 Allocation/Deallocation + RAII   | ✅/❌           | ❌/❌         | Memory management with automatic cleanup                        |
| &nbsp;&nbsp;&nbsp;&nbsp;1.5 Utilities (occupied, free, etc.) | ✅/❌           | ❌/❌         | Helper functions for memory status checks                       |
| **2. Code Interaction**                                      |
| &nbsp;&nbsp;&nbsp;&nbsp;2.1 Signature Scanning               | ✅/❌           | ❌/❌         | Find specific byte patterns in memory                           |
| &nbsp;&nbsp;&nbsp;&nbsp;2.2 Function Invoking                | ✅/❌           | ❌/❌         | Call functions dynamically at runtime                           |
| &nbsp;&nbsp;&nbsp;&nbsp;2.3 Hooking                          | ❌/❌           | ❌/❌         | Intercept function calls                                        |
| &nbsp;&nbsp;&nbsp;&nbsp;2.4 Disassembling                    | ❌/❌           | ❌/❌         | Getting information about ASM instructions                      |
| **3. Module & Process**                                      |
| &nbsp;&nbsp;&nbsp;&nbsp;3.1 Module Interaction               | ✅/❌           | ❌/❌         | Work with loaded modules (DLLs/shared libraries)                |
| &nbsp;&nbsp;&nbsp;&nbsp;3.2 Module Enumeration               | ❌/❌           | ❌/❌         | List all loaded modules in a process                            |
| &nbsp;&nbsp;&nbsp;&nbsp;3.3 Process Enumeration              | ❌/❌           | ❌/❌         | List all running processes                                      |
| **4. Threading**                                             |
| &nbsp;&nbsp;&nbsp;&nbsp;4.1 Thread-safety                    | ❌/❌           | ❌/❌         | Ensure operations are safe in multi-threaded environments       |
| &nbsp;&nbsp;&nbsp;&nbsp;4.2 Thread-enumeration               | ❌/❌           | ❌/❌         | List all threads in a process                                   |
| **5. Advanced Features**                                     |
| &nbsp;&nbsp;&nbsp;&nbsp;5.1 Segment Interaction              | ❌/❌           | ❌/❌         | Work with specific memory segments (.text, .data, etc.)         |
| &nbsp;&nbsp;&nbsp;&nbsp;5.2 Virtual Table Interaction        | ❌/❌           | ❌/❌         | Manipulate C++ virtual function tables                          |
| &nbsp;&nbsp;&nbsp;&nbsp;5.3 Bit Utilities                    | ❌/❌           | ❌/❌         | Bit-level operations and manipulations                          |
| **6. System**                                                |
| &nbsp;&nbsp;&nbsp;&nbsp;6.1 System Info                      | ✅/✅           | ❌/❌         | Retrieve system-specific information (e.g., page size)          |
| &nbsp;&nbsp;&nbsp;&nbsp;6.2 Cross-platform error-handling    | ✅/✅           | ❌/❌         | Unified error handling across different platforms               |

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
