add_library(rendershop-base INTERFACE)

target_sources(
	rendershop-base
	INTERFACE "include/rsp/base/node.hpp"
				"include/rsp/base/algorithm_node.hpp"
				"include/rsp/base/constant_node.hpp"
				"include/rsp/base/algorithm.hpp"
				"include/rsp/base/port.hpp"
				"include/rsp/base/sentinel.hpp"
				"include/rsp/base/graph.hpp"
)

target_include_directories(rendershop-base INTERFACE "include")

target_link_libraries(rendershop-base INTERFACE rendershop-util)
