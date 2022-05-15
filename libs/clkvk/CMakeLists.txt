add_library(
	clkvk "src/init.cpp" "include/clk/clkvk/init.hpp" "include/clk/clkvk/window.hpp" "include/clk/clkvk/instance.hpp"
)

target_include_directories(clkvk PUBLIC "include")

target_link_libraries(clkvk PUBLIC clayknot::base Vulkan::Vulkan glfw)

install(TARGETS clkvk)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::clkvk ALIAS clkvk)
