add_executable(rendershop-editor
	"src/main.cpp"
)

target_include_directories(rendershop-editor
	PRIVATE
	"include"
	"../../external/glm/include"
)

target_link_libraries(rendershop-editor
	PRIVATE
	rendershop-base
	rendershop-nodes
	rendershop-gui
	glad
	glfw
	imgui
	imgui-node-editor
)