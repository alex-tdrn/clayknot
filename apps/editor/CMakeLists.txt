add_executable(editor "src/main.cpp")

target_link_libraries(
	editor
	PRIVATE clayknot::base
			clayknot::algorithms
			clayknot::gui
			glad::glad
			glfw
			imgui
			imnodes
			implot
)

install(TARGETS editor)
