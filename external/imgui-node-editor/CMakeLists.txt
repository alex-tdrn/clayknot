project(imgui-node-editor)

add_library(imgui-node-editor
	"src/crude_json.cpp"
	"src/imgui_canvas.cpp"
	"src/imgui_node_editor.cpp"
	"src/imgui_node_editor_api.cpp"
	"include/crude_json.h"
	"include/imgui_bezier_math.h"
	"include/imgui_bezier_math.inl"
	"include/imgui_canvas.h"
	"include/imgui_extra_math.h"
	"include/imgui_extra_math.inl"
	"include/imgui_node_editor.h"
	"include/imgui_node_editor_internal.h"
	"include/imgui_node_editor_internal.inl"
)

target_include_directories(imgui-node-editor
	PUBLIC
	"include"
)

target_link_libraries(imgui-node-editor
	PUBLIC
	imgui
)