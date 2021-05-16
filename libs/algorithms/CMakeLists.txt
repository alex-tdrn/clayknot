add_library(
	rendershop-algorithms
	"src/decompose_color.cpp"
	"include/rsp/algorithms/decompose_color.h"
	"src/grayscale_color_node.cpp"
	"include/rsp/algorithms/grayscale_color_node.h"
	"src/mix_colors.cpp"
	"include/rsp/algorithms/mix_colors.h"
	"src/random_color_source.cpp"
	"include/rsp/algorithms/random_color_source.h"
	"src/value_to_color.cpp"
	"include/rsp/algorithms/value_to_color.h"
)

target_include_directories(rendershop-algorithms PUBLIC "include")

target_link_libraries(rendershop-algorithms PUBLIC rendershop-base glm::glm)
