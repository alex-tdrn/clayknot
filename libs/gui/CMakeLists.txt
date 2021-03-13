add_library(
	rendershop-gui
	"src/Init.cpp"
	"src/Panel.cpp"
	"src/widgets/GraphViewer.cpp"
	"src/widgets/GraphEditor.cpp"
	"include/rsp/gui/Init.h"
	"include/rsp/gui/Panel.h"
	"include/rsp/gui/widgets/Widget.hpp"
	"include/rsp/gui/widgets/Viewer.hpp"
	"include/rsp/gui/widgets/Editor.hpp"
	"include/rsp/gui/widgets/GraphViewer.h"
	"include/rsp/gui/widgets/GraphEditor.h"
)

target_include_directories(rendershop-gui PUBLIC "include")

target_compile_definitions(rendershop-gui PUBLIC IMGUI_ADD_VECTOR_CONVERSIONS)

target_link_libraries(rendershop-gui PUBLIC rendershop-base rendershop-algorithms imgui imnodes)
