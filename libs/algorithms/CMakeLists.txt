add_library(
	algorithms "src/init.cpp ""src/color.cpp" "include/clk/algorithms/init.hpp" "include/clk/algorithms/color.hpp"
)

target_include_directories(algorithms PUBLIC "include")

target_link_libraries(algorithms PUBLIC clayknot::base glm::glm range-v3::range-v3)

install(TARGETS algorithms)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::algorithms ALIAS algorithms)
