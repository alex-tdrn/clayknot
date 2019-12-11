add_executable(tests_gui
	"include/WidgetPanel.h"
	"src/WidgetPanel.cpp"
	"src/main.cpp"
)

target_include_directories(tests_gui
	PRIVATE
	"include"
)

target_link_libraries(tests_gui
	PRIVATE
	rendershop-base
	rendershop-nodes
	rendershop-gui
	glad
	glfw
)