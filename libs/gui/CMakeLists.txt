add_library(rendershop-gui
	"src/panels/Panel.cpp"
	"include/rshp/gui/panels/Panel.h"

	"src/panels/RootPanel.cpp"
	"include/rshp/gui/panels/RootPanel.h"

	"src/panels/StyleEditor.cpp"
	"include/rshp/gui/panels/StyleEditor.h"

	"src/panels/NodeEditor.cpp"
	"include/rshp/gui/panels/NodeEditor.h"

	"src/nodes/FrameControllerNode.cpp"
	"include/rshp/gui/nodes/FrameControllerNode.h"

	"src/nodes/InputDataPort.cpp"
	"include/rshp/gui/nodes/InputDataPort.h"

	"src/nodes/InputEventPort.cpp"
	"include/rshp/gui/nodes/InputEventPort.h"

	"src/nodes/InputPort.cpp"
	"include/rshp/gui/nodes/InputPort.h"

	"src/nodes/Node.cpp"
	"include/rshp/gui/nodes/Node.h"

	"src/nodes/OutputDataPort.cpp"
	"include/rshp/gui/nodes/OutputDataPort.h"

	"src/nodes/OutputDataPortDelegate.cpp"
	"include/rshp/gui/nodes/OutputDataPortDelegate.h"

	"src/nodes/OutputEventPort.cpp"
	"include/rshp/gui/nodes/OutputEventPort.h"

	"src/nodes/OutputPort.cpp"
	"include/rshp/gui/nodes/OutputPort.h"

	"include/rshp/gui/Animation.hpp"
	"include/rshp/gui/ImGuiUtilities.hpp"
	"include/rshp/gui/Stylesheet.hpp"
	"include/rshp/gui/UIDebug.hpp"
	"include/rshp/gui/UniqueID.hpp"
)

target_include_directories(rendershop-gui
	PUBLIC
	"include"
	"../../external/glm/include"
)

target_compile_definitions(rendershop-gui
	PUBLIC 
	IMGUI_ADD_VECTOR_CONVERSIONS
)

target_link_libraries(rendershop-gui
	PRIVATE
	rendershop-base
	rendershop-nodes
	imgui
	imgui-node-editor
)