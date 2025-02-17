add_executable(
  "mywr_tests" "shared/address.cpp" "internal/protect.cpp" "internal/llmo.cpp"
               "internal/patch.cpp" "internal/scanner.cpp")

target_compile_features(mywr_tests PRIVATE cxx_std_17)
target_link_libraries(mywr_tests PRIVATE mywr GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(mywr_tests)
