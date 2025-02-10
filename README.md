# mywr

Cross-platform multiarchitectural (in future) memhacking header-online library written in C++17. The next generation of my [memwrapper](https://github.com/The-Musaigen/memwrapper) project.

## Features

* Memory region unprotection.
* Read/write/copy/fill/compare memory.
* RAII classes memory patching, unprotecting.
* Signature scanning.

## Platforms / Architectures

| Status | Description                                                             |
| ------ | ----------------------------------------------------------------------- |
| ✅      | Supported and tested.                                                   |
| 💻      | In developing.                                                          |
| ❌      | Unsupported.                                                            |
| ❓      | Maybe supported but untested.                                           |
| ❗      | Not in plans. (Due to lack of technical/physical/material capabilities) |

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

## Usage / Demo

For usage examples, you can look into the `tests` directory or the `demo` directory.

## Installation

1. Clone repository (using FetchContent, or just download the repo).
2. Extract `include` directory content and include `mywr.hpp` (or just include `mywr.hpp` if you used FetchContent).

## License

`mywr` licensed under `MIT` license. See `LICENSE` file to details.
