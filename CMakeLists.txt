cmake_minimum_required(VERSION 3.20)

project(clayknot VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(cmake/Sanitizers.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/ConanAutoInstall.cmake)
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
add_subdirectory("apps")
