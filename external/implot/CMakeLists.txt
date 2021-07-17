add_library(
	implot "include/implot.h" "src/implot.cpp" "src/implot_items.cpp" "src/implot_demo.cpp"
			"src/internal/implot_internal.h"
)

target_include_directories(
	implot SYSTEM
	PUBLIC "include"
	PRIVATE "src/internal"
)

target_link_libraries(implot PUBLIC imgui)

add_library(clayknot::implot ALIAS implot)
