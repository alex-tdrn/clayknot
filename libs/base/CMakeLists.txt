add_library(base INTERFACE)

target_sources(
	base
	INTERFACE "include/clk/base/node.hpp"
				"include/clk/base/algorithm_node.hpp"
				"include/clk/base/constant_node.hpp"
				"include/clk/base/algorithm.hpp"
				"include/clk/base/port.hpp"
				"include/clk/base/input.hpp"
				"include/clk/base/output.hpp"
				"include/clk/base/sentinel.hpp"
				"include/clk/base/graph.hpp"
)

target_include_directories(base INTERFACE "include")

target_link_libraries(base INTERFACE clayknot::util range-v3::range-v3)

install(TARGETS base)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::base ALIAS base)
