add_library(rendershop-base INTERFACE)

target_sources(
	rendershop-base
	INTERFACE "include/rsp/base/Node.hpp"
				"include/rsp/base/AlgorithmNode.hpp"
				"include/rsp/base/ConstantNode.hpp"
				"include/rsp/base/Algorithm.hpp"
				"include/rsp/base/Port.hpp"
				"include/rsp/base/Sentinel.hpp"
				"include/rsp/base/Graph.hpp"
)

target_include_directories(rendershop-base INTERFACE "include")

target_link_libraries(rendershop-base INTERFACE rendershop-util)
