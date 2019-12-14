add_executable(benchmarks_base
	"src/callbacks.cpp"
	"src/main.cpp"
)

target_link_libraries(benchmarks_base
	PRIVATE
	Catch2::Catch2
	rendershop-base
)