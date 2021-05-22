if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_EXPORT_COMPILE_COMMANDS)
	add_custom_target(
		copy-compile-commands ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/compile_commands.json
									${CMAKE_SOURCE_DIR}/build
	)
endif()
