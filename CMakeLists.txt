cmake_minimum_required(VERSION 3.18)

include(cmake/BuildTypesConfig.cmake)
project(rendershop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

find_program(CONAN_FOUND conan)
if(CONAN_FOUND)
	set(CONAN_BUILD_TYPE "Debug")
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		set(CONAN_BUILD_TYPE "Release")
	endif()

	execute_process(
		COMMAND conan install ${CMAKE_SOURCE_DIR} -if ${CMAKE_BINARY_DIR} --build missing -s
				build_type=${CONAN_BUILD_TYPE}
	)
endif()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake OPTIONAL)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if(supported)
	message(STATUS "IPO / LTO enabled for release builds")
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
	message(STATUS "IPO / LTO not supported: <${error}>")
endif()

add_subdirectory("external")
add_subdirectory("libs")
add_subdirectory("tests")
# add_subdirectory("benchmarks")
add_subdirectory("apps")
