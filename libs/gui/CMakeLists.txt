add_library(
	rendershop-gui
	"include/rsp/gui/init.h"
	"include/rsp/gui/panel.h"
	"include/rsp/gui/widgets/widget.hpp"
	"include/rsp/gui/widgets/viewer.hpp"
	"include/rsp/gui/widgets/editor.hpp"
	"include/rsp/gui/widgets/graph_viewer.h"
	"include/rsp/gui/widgets/graph_editor.h"
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
	rendershop-gui
	PUBLIC "include"
	PRIVATE "src/internal"
)

target_compile_definitions(rendershop-gui PUBLIC IMGUI_ADD_VECTOR_CONVERSIONS)

target_link_libraries(rendershop-gui PUBLIC rendershop-base rendershop-algorithms imgui imnodes)
