# mywr

Cross-platform multiarchitectural (in future) memhacking header-online library written in C++17. The next generation of my [memwrapper](https://github.com/The-Musaigen/memwrapper) project.

## Features

* Memory region unprotection.
* Read/write/copy/fill/compare memory.
* RAII classes memory patching, unprotecting.
* Signature scanning.

## Platforms

* ✅ means supported and tested.
* ❌ means unsupported.
* ⚠️ means maybe supported but untested.
* 💻 means in developing.

> [!NOTE]
Even though platforms such as "macOS" and "Android" are listed, this does not mean that in the future the task will be to implement their support in the library due to the lack of physical/material capabilities. It's worth taking this only as a cheat sheet, not a list of tasks.

| Platform        | Status |
| --------------- | ------ |
| Windows         | 💻      |
| Linux           | ❌      |
| Linux (FreeBSD) | ❌      |
| Andorid         | ❌      |
| macOS           | ❌      |

## Architectures

* ✅ means supported and tested.
* ❌ means unsupported.
* ⚠️ means maybe supported but untested.
* 💻 means in developing.

> [!NOTE]
Same note as above.

| Architecture    | Status |
| --------------- | ------ |
| x86             | 💻      |
| x86_64          | 💻      |
| ARM             | ❌      |
| ARM64 (Aarch64) | ❌      |

## Usage / Demo

For usage examples, you can look into the `tests` directory or the `demo` directory.

## Installation

1. Clone repository (using FetchContent, or just download the repo).
2. Extract `include` directory content and include `mywr.hpp` (or just include `mywr.hpp` if you used FetchContent).

## License

`mywr` licensed under `MIT` license. See `LICENSE` file to details.
