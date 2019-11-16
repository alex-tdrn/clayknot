add_executable(tests
	"src/base_pipes.cpp"
	"src/base_ports.cpp"
	"src/main.cpp"
)

target_include_directories(tests
	PRIVATE
	"include"
)

target_link_libraries(tests
	PRIVATE
	rendershop-pipes
	glad
	glfw
)