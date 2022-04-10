cmake_minimum_required(VERSION 3.20)

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

project(clayknot VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(cmake/StaticAnalyzers.cmake)

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
