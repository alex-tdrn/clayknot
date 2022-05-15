add_library(gui)

target_sources(
	gui
	PRIVATE "src/init.cpp"
			"src/panel.cpp"
			"src/widgets/widget.cpp"
			"src/widgets/data_widget.cpp"
			"src/widgets/graph_viewer.cpp"
			"src/widgets/graph_editor.cpp"
			"src/widgets/widget_tree.cpp"
			"src/widgets/profiler_editor.cpp"
			"src/widgets/action_widget.cpp"
			"src/internal/layout_solver.cpp"
			"src/internal/port_viewers.cpp"
			"src/internal/port_editors.cpp"
			"src/internal/node_viewers.cpp"
			"src/internal/node_editors.cpp"
)

target_include_directories(
	gui
	PUBLIC "include"
	PRIVATE "src/internal"
)

target_compile_definitions(gui PUBLIC IMGUI_ADD_VECTOR_CONVERSIONS)

target_link_libraries(
	gui
	PUBLIC clayknot::util
			clayknot::base
			clayknot::algorithms
			range-v3::range-v3
			imgui
			imnodes
			implot
)

install(TARGETS gui)
install(DIRECTORY include/ DESTINATION include)

add_library(clayknot::gui ALIAS gui)
