add_library(algorithms)

target_sources(algorithms PRIVATE "src/init.cpp" "src/boolean.cpp" "src/math.cpp" "src/color.cpp")

target_include_directories(algorithms PUBLIC "include")

target_link_libraries(algorithms PUBLIC clayknot::base glm::glm range-v3::range-v3)

install(TARGETS algorithms)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::algorithms ALIAS algorithms)
