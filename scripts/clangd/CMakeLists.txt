# Auto-generator of clangd configuration based on current architecture, os,
# etc...

set(CLANGD_PATH "${PROJECT_SOURCE_DIR}/.clangd")
set(CLANGD_MAIN_FLAGS
    "-std=c++17,
    -Wall,
    -Wextra,
    -Wno-c++98-compat,
    -Wno-c++98-compat-pedantic,
    -Wno-unused-function,
    -Wno-unused-template,
    -Wno-covered-switch-default")
set(CLANGD_M32 "-m32")

# GCC 13 used in mywr
set(CLANGD_GCC_FLAGS
    "
  -I/usr/include/c++/13,
  -I/usr/include/x86_64-linux-gnu/c++/13,
  -I/usr/include/c++/13/backward,
  -I/usr/lib/gcc/x86_64-linux-gnu/13/include,
  -I/usr/local/include,
  -I/usr/include/x86_64-linux-gnu,
  -I/usr/include
")

# Detecting architecture.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CLANGD_FLAGS "${CLANGD_MAIN_FLAGS}")
else()
  set(CLANGD_FLAGS "${CLANGD_MAIN_FLAGS},
    ${CLANGD_M32}")
endif()

# Detecting OS and compiler.
if(UNIX)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CLANGD_FLAGS "${CLANGD_FLAGS}, ${CLANGD_GCC_FLAGS}")
  endif()
endif()

# Create the body of .clangd file.
set(CLANGD_BODY
    "CompileFlags:
  CompilationDatabase: build
  Add: [${CLANGD_FLAGS}]")

# Rewrite or create .clangd file.
file(WRITE ${CLANGD_PATH} ${CLANGD_BODY})
