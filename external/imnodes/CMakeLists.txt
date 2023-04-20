add_library(imnodes "include/imnodes.h" "src/imnodes.cpp" "src/internal/imnodes_internal.h")

target_include_directories(
    imnodes SYSTEM
    PUBLIC "include"
    PRIVATE "src/internal")

target_link_libraries(imnodes PUBLIC imgui::imgui)

target_compile_definitions(imnodes PUBLIC "IMGUI_DEFINE_MATH_OPERATORS")

add_library(clayknot::imnodes ALIAS imnodes)
