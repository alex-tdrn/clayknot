add_library(util)

target_sources(util PRIVATE "src/color_rgb.cpp" "src/color_rgba.cpp" "src/profiler.cpp" "src/timestamp.cpp")

target_include_directories(util PUBLIC "include")
target_link_libraries(util PUBLIC glm::glm range-v3::range-v3)

install(TARGETS util)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::util ALIAS util)
