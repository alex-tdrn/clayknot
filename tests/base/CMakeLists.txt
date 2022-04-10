add_executable(tests_base "src/ports.cpp" "src/nodes.cpp")

target_link_libraries(tests_base PRIVATE Catch2::Catch2WithMain clayknot::base)
target_compile_definitions(tests_base PRIVATE CATCH_CONFIG_CONSOLE_WIDTH=200)
