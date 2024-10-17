cmake_minimum_required(VERSION 3.14)

enable_testing()
add_executable(memwrapper-tests "address_test.cpp" "protect_test.cpp" "llmo_test.cpp" "traits_test.cpp" "invoker_test.cpp" "disassembler_test.cpp" "proc_test.cpp" "hook_test.cpp")
target_link_libraries(memwrapper-tests gtest gtest_main ${PROJECT_NAME})
target_compile_features(memwrapper-tests PUBLIC cxx_std_17)

include(GoogleTest)
gtest_discover_tests(memwrapper-tests)


