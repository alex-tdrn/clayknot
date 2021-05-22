add_library(
	algorithms
	"src/decompose_color.cpp"
	"include/clk/algorithms/decompose_color.h"
	"src/grayscale_color_node.cpp"
	"include/clk/algorithms/grayscale_color_node.h"
	"src/mix_colors.cpp"
	"include/clk/algorithms/mix_colors.h"
	"src/random_color_source.cpp"
	"include/clk/algorithms/random_color_source.h"
	"src/value_to_color.cpp"
	"include/clk/algorithms/value_to_color.h"
)

target_include_directories(algorithms PUBLIC "include")

target_link_libraries(algorithms PUBLIC clayknot::base glm::glm)

install(TARGETS algorithms)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::algorithms ALIAS algorithms)
