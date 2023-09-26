cmake_minimum_required(VERSION 3.27)

project(clayknot VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(cmake/CompilerWarnings.cmake)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if(supported)
    message(STATUS "IPO / LTO enabled for release builds")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()

if(MSVC)
    add_compile_options("/bigobj")
endif()

add_subdirectory("external")
add_subdirectory("libs")
add_subdirectory("apps")

option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory("tests")
endif()
