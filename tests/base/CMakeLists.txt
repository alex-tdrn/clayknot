add_executable(tests_base "src/main.cpp" "src/ports.cpp" "src/nodes.cpp")

target_link_libraries(tests_base PRIVATE Catch2::Catch2 rendershop-base)
