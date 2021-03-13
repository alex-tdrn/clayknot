add_executable(rendershop-editor "src/main.cpp")

target_include_directories(rendershop-editor PRIVATE "include" "../../external/glm/include")

target_link_libraries(
	rendershop-editor
	PRIVATE rendershop-base
			rendershop-algorithms
			rendershop-gui
			glad::glad
			glfw::glfw
			imgui
			imnodes
)
