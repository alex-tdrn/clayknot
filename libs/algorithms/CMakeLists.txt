add_library(
	algorithms
	"src/decompose_color.cpp"
	"include/clk/algorithms/decompose_color.hpp"
	"src/grayscale_color_node.cpp"
	"include/clk/algorithms/grayscale_color_node.hpp"
	"src/mix_colors.cpp"
	"include/clk/algorithms/mix_colors.hpp"
	"src/random_color_source.cpp"
	"include/clk/algorithms/random_color_source.hpp"
	"src/value_to_color.cpp"
	"include/clk/algorithms/value_to_color.hpp"
)

target_include_directories(algorithms PUBLIC "include")

target_link_libraries(algorithms PUBLIC clayknot::base glm::glm range-v3::range-v3)

install(TARGETS algorithms)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::algorithms ALIAS algorithms)
