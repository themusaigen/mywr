add_executable("demo")

target_compile_features("demo" PRIVATE cxx_std_17)
target_link_libraries("demo" PRIVATE "mywr")
target_sources("demo" PRIVATE "main.cpp" "address_showcase.cpp"
                              "protect_showcase.cpp" "llmo_showcase.cpp")
