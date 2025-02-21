# Add an executable target named "demo"
add_executable("demo")

# Set the C++ standard for the target to C++17
target_compile_features("demo" PRIVATE cxx_std_17)

# Link the "demo" target with the "mywr" library
target_link_libraries("demo" PRIVATE "mywr")

# Set the source files for the "demo" target
target_sources("demo" PRIVATE "main.cpp" "address_showcase.cpp"
                              "protect_showcase.cpp" "llmo_showcase.cpp")
