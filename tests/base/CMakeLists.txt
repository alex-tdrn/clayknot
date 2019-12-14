add_executable(tests_base
	"src/nodes.cpp"
	"src/ports.cpp"
	"src/main.cpp"
)

target_link_libraries(tests_base
	PRIVATE
	Catch2::Catch2
	rendershop-base
)