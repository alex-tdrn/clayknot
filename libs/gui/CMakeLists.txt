add_library(
	rendershop-gui
	"src/panels/Panel.cpp"
	"include/rsp/gui/panels/Panel.h"
	"src/panels/RootPanel.cpp"
	"include/rsp/gui/panels/RootPanel.h"
	"src/panels/StyleEditor.cpp"
	"include/rsp/gui/panels/StyleEditor.h"
	"src/panels/NodeEditor.cpp"
	"include/rsp/gui/panels/NodeEditor.h"
	"src/nodes/InputPort.cpp"
	"include/rsp/gui/nodes/InputPort.h"
	"src/nodes/Node.cpp"
	"include/rsp/gui/nodes/Node.h"
	"src/nodes/OutputPort.cpp"
	"include/rsp/gui/nodes/OutputPort.h"
	"include/rsp/gui/widgets/Widget.hpp"
	"include/rsp/gui/widgets/Viewer.hpp"
	"include/rsp/gui/widgets/Editor.hpp"
	"include/rsp/gui/Animation.hpp"
	"include/rsp/gui/ImGuiUtilities.hpp"
	"include/rsp/gui/Stylesheet.hpp"
	"include/rsp/gui/UIDebug.hpp"
	"include/rsp/gui/UniqueID.hpp"
)

target_include_directories(rendershop-gui PUBLIC "include")

target_compile_definitions(rendershop-gui PUBLIC IMGUI_ADD_VECTOR_CONVERSIONS)

target_link_libraries(rendershop-gui PUBLIC rendershop-base rendershop-nodes imgui imgui-node-editor)
