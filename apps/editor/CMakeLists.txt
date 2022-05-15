add_executable(editor "src/main.cpp")

target_link_libraries(
	editor
	PRIVATE clayknot::base
			clayknot::algorithms
			clayknot::gui
			clayknot::clkvk
			glad::glad
			glfw
			imgui
			imnodes
			implot
)

install(TARGETS editor)
