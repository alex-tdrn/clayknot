add_library(imnodes "include/imnodes.h" "src/imnodes.cpp")

target_include_directories(imnodes SYSTEM PUBLIC "include")

target_link_libraries(imnodes PUBLIC imgui)

add_library(clayknot::imnodes ALIAS imnodes)
