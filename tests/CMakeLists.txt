find_package(Catch2 REQUIRED)

enable_extra_compiler_warnings()

add_executable(tests "src/base/nodes.cpp" "src/base/ports.cpp" "src/util/colors.cpp")

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain clayknot::util clayknot::base)
target_compile_definitions(tests PRIVATE CATCH_CONFIG_CONSOLE_WIDTH=200)
