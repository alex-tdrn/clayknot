add_library(base)

target_sources(
	base
	PRIVATE "src/node.cpp"
			"src/algorithm_node.cpp"
			"src/constant_node.cpp"
			"src/algorithm.cpp"
			"src/port.cpp"
			"src/input.cpp"
			"src/output.cpp"
			"src/graph.cpp"
)

target_include_directories(base PUBLIC "include")

target_link_libraries(base PUBLIC clayknot::util range-v3::range-v3)

install(TARGETS base)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::base ALIAS base)
