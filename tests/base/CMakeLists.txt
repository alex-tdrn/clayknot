add_executable(tests_base
	"src/nodes.cpp"
	"src/ports.cpp"
	"src/main.cpp"
)

target_include_directories(tests_base
	PRIVATE
	"include"
)

target_link_libraries(tests_base
	PRIVATE
	rendershop-base
	rendershop-nodes
	glad
	glfw
)