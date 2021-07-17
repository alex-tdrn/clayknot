add_library(
	gui
	"include/clk/gui/init.hpp"
	"include/clk/gui/panel.hpp"
	"include/clk/gui/widgets/widget.hpp"
	"include/clk/gui/widgets/viewer.hpp"
	"include/clk/gui/widgets/editor.hpp"
	"include/clk/gui/widgets/graph_viewer.hpp"
	"include/clk/gui/widgets/graph_editor.hpp"
	"include/clk/gui/widgets/profiler_viewer.hpp"
	"src/init.cpp"
	"src/panel.cpp"
	"src/widgets/graph_viewer.cpp"
	"src/widgets/graph_editor.cpp"
	"src/internal/widget_cache.hpp"
	"src/internal/selection_manager.hpp"
	"src/internal/node_viewers.hpp"
	"src/internal/port_viewers.hpp"
	"src/internal/node_editors.hpp"
	"src/internal/port_editors.hpp"
)

target_include_directories(
	gui
	PUBLIC "include"
	PRIVATE "src/internal"
)

target_compile_definitions(gui PUBLIC IMGUI_ADD_VECTOR_CONVERSIONS)

target_link_libraries(gui PUBLIC clayknot::base clayknot::algorithms range-v3::range-v3 imgui imnodes implot)

install(TARGETS gui)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::gui ALIAS gui)
