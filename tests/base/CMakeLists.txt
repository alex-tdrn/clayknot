add_executable(tests_base "src/main.cpp" "src/ports.cpp" "src/nodes.cpp")

target_link_libraries(tests_base PRIVATE Catch2::Catch2 rendershop-base)
target_compile_definitions(tests_base PRIVATE CATCH_CONFIG_CONSOLE_WIDTH=200)