cmake_minimum_required(VERSION 3.15)

project(rendershop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_subdirectory("external")
add_subdirectory("libs")
add_subdirectory("tests")
add_subdirectory("apps")