
# mywr

A wrapper for memory manipulation. Includes: low-level memory operations (read, write, copy, fill, compare). Next version of old [memwrapper](https://github.com/The-Musaigen/memwrapper).

[![Windows](https://github.com/themusaigen/mywr/actions/workflows/windows-compile-check.yaml/badge.svg)](https://github.com/themusaigen/mywr/actions/workflows/windows-compile-check.yaml)
[![Linux](https://github.com/themusaigen/mywr/actions/workflows/linux-compile-check.yaml/badge.svg)](https://github.com/themusaigen/mywr/actions/workflows/linux-compile-check.yaml)
[![Format](https://github.com/themusaigen/mywr/actions/workflows/format-check.yaml/badge.svg)](https://github.com/themusaigen/mywr/actions/workflows/format-check.yaml)
[![Doxygen](https://github.com/themusaigen/mywr/actions/workflows/upload-docs.yaml/badge.svg)](https://github.com/themusaigen/mywr/actions/workflows/upload-docs.yaml)

## Installation

### Basic way

Clone repository and include `mywr.hpp`. C++20 required.

### FetchContent
Install `mywr` using `FetchContent`.

```
include(FetchContent)
FetchContent_Declare(
    mywr
    GIT_REPOSTITORY https://github.com/themusaigen/mywr/mywr.git
    GIT_TAG <specify github tag here>
)
FetchContent_MakeAvailable(mywr)
```

In root `CMakeLists.txt`

```
target_link_libraries(${PROJECT_NAME} <access modifier> mywr)
```

## Usage/Examples

See examples in `tests` folder.

## License

[MIT](https://choosealicense.com/licenses/mit/)

