#include "clk/algorithms/boolean.hpp"
#include "clk/algorithms/color.hpp"
#include "clk/algorithms/init.hpp"
#include "clk/base/algorithm_node.hpp"
#include "clk/base/constant_node.hpp"
#include "clk/base/graph.hpp"
#include "clk/base/input.hpp"
#include "clk/base/node.hpp"
#include "clk/base/output.hpp"
#include "clk/base/port.hpp"
#include "clk/gui/init.hpp"
#include "clk/gui/panel.hpp"
#include "clk/gui/widgets/composite_editor.hpp"
#include "clk/gui/widgets/composite_viewer.hpp"
#include "clk/gui/widgets/data_reader.hpp"
#include "clk/gui/widgets/data_writer.hpp"
#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/viewer.hpp"
#include "clk/gui/widgets/widget.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/gui/widgets/widget_tree.hpp"
#include "clk/util/color_rgba.hpp"
#include "clk/util/profiler.hpp"
#include "clk/util/timestamp.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cstddef>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imnodes.h>
#include <implot.h>
#include <iostream>
#include <memory>
#include <range/v3/view/view.hpp>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

void APIENTRY gl_debug_output(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/,
	const GLchar* message, const void* /*userParam*/);
void im_gui_cherry_style();

auto main(int /*argc*/, char** /*argv*/) -> int
{
	try
	{
		if(glfwInit() == 0)
			return 1;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

		GLFWwindow* window = glfwCreateWindow(1920, 1080, "clayknot", nullptr, nullptr);
		if(window == nullptr)
			return 1;

		glfwMakeContextCurrent(window);

		if(gladLoadGL() == 0)
			return 1;
#ifndef NDEBUG
		GLint context_flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
		if((context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(gl_debug_output, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
#endif

		glfwSwapInterval(1);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 420");

		ImNodes::CreateContext();
		ImNodes::GetIO().AltMouseButton = ImGuiMouseButton_Right;
		auto& style = ImNodes::GetStyle();
		// TODO these colors need to come form a stylesheet
		style.Colors[ImNodesCol_LinkHovered] = clk::color_rgba{1.0f}.packed();
		style.Colors[ImNodesCol_LinkSelected] = clk::color_rgba{1.0f}.packed();
		style.Colors[ImNodesCol_Link] = clk::color_rgba{0.0f}.packed();
		style.Colors[ImNodesCol_PinHovered] = clk::color_rgba{1.0f}.packed();

		style.Colors[ImNodesCol_TitleBarHovered] = style.Colors[ImNodesCol_TitleBar];
		style.Colors[ImNodesCol_TitleBarSelected] = style.Colors[ImNodesCol_TitleBar];
		style.Colors[ImNodesCol_NodeBackgroundHovered] = style.Colors[ImNodesCol_NodeBackground];
		style.Colors[ImNodesCol_NodeBackgroundSelected] = style.Colors[ImNodesCol_NodeBackground];

		ImPlot::CreateContext();

		clk::algorithms::init();
		auto widget_factory = clk::gui::create_default_factory();

		struct test_struct
		{
			int a = 2;
			float b = 3.0;
			clk::color_rgba c = clk::color_rgba::create_random();
			clk::graph g = []() -> clk::graph {
				clk::graph ret;
				ret.add_node(std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::boolean_and>()));
				ret.add_node(std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::boolean_nand>()));
				ret.add_node(std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::boolean_or>()));
				ret.add_node(std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::boolean_xor>()));
				ret.add_node([]() {
					auto node = std::make_unique<clk::constant_node>();
					node->add_output(std::make_unique<clk::output_of<bool>>());
					node->add_output(std::make_unique<clk::output_of<bool>>());

					return node;
				}());

				return ret;
			}();
		};
		test_struct composite_data;

		widget_factory->register_viewer<test_struct>(
			[](clk::gui::data_reader<test_struct> reader, std::shared_ptr<clk::gui::widget_factory> factory,
				std::string_view name) {
				auto viewer = std::make_unique<clk::gui::composite_viewer_of<test_struct>>(std::move(factory), name);
				viewer->set_data_reader(std::move(reader));

				viewer->add_sub_viewer(&test_struct::a, "a viewer");
				viewer->add_sub_viewer(&test_struct::b, "b viewer");
				viewer->add_sub_viewer<float>(
					[result = 0.0f](test_struct const& t) mutable -> float const* {
						result = static_cast<float>(t.a) + t.b;
						return &result;
					},
					"a + b viewer");
				viewer->add_sub_viewer(&test_struct::c, "c viewer");
				viewer->add_sub_viewer(&test_struct::g, "g viewer");

				return viewer;
			});

		widget_factory->register_editor<test_struct>(
			[](clk::gui::data_writer<test_struct> writer, std::shared_ptr<clk::gui::widget_factory> factory,
				std::string_view name) {
				auto editor = std::make_unique<clk::gui::composite_editor_of<test_struct>>(std::move(factory), name);
				editor->set_data_writer(std::move(writer));

				editor->add_sub_editor(&test_struct::a, "a editor");
				editor->add_sub_editor(&test_struct::b, "b editor");
				editor->add_sub_editor(&test_struct::c, "c editor");
				editor->add_sub_editor(&test_struct::g, "g editor");

				return editor;
			});

		clk::gui::panel::create_orphan(widget_factory->create(std::as_const(composite_data), "composite data viewer"));

		clk::gui::panel::create_orphan(widget_factory->create(composite_data, "composite data editor"));

		auto graph1 = []() -> clk::graph {
			auto random_color =
				std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::random_color>());
			auto decompose_color =
				std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::decompose_color>());
			auto value_to_color =
				std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::value_to_color>());
			auto mix_colors = std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::mix_colors>());

			clk::graph ret;

			ret.add_node(std::move(random_color));
			ret.add_node(std::move(decompose_color));
			ret.add_node(std::move(value_to_color));
			ret.add_node(std::move(mix_colors));
			return ret;
		}();

		clk::gui::panel::create_orphan(widget_factory->create(std::as_const(graph1), "graph1 viewer"));

		clk::gui::panel::create_orphan(widget_factory->create(graph1, "graph1 editor"));

		clk::profiler profiler_frame;
		auto frametimes_panel = clk::gui::panel();
		frametimes_panel.set_widget(widget_factory->create(profiler_frame, "Frametimes"));
		frametimes_panel.set_title_bar_visibility(false);
		frametimes_panel.set_resizability(clk::gui::panel::resizability::automatic);
		frametimes_panel.set_docking(false);
		frametimes_panel.set_movability(false);
		frametimes_panel.set_interactivity(false);
		frametimes_panel.set_opactiy(0.5f);

		clk::profiler profiler_empty;
		profiler_empty.set_active(false);
		clk::profiler profiler_draw;
		profiler_draw.set_active(false);
		clk::profiler profiler_swap;
		profiler_swap.set_active(false);

		{
			auto profiler_tree = std::make_unique<clk::gui::widget_tree>("Extra profilers");
			profiler_tree->get_subtree("Profiler overhead")
				.add(widget_factory->create(profiler_empty, "Profiler overhead"));
			profiler_tree->get_subtree("GUI Draw").add(widget_factory->create(profiler_draw, "GUI Draw"));
			profiler_tree->get_subtree("Swap buffers").add(widget_factory->create(profiler_swap, "Swap buffers"));
			clk::gui::panel::create_orphan(std::move(profiler_tree));
		}

		while(glfwWindowShouldClose(window) == 0)
		{
			profiler_frame.record_sample_end();
			profiler_frame.record_sample_start();

			profiler_empty.record_sample_start();
			profiler_empty.record_sample_end();

			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			int display_w = 0;
			int display_h = 0;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			profiler_draw.record_sample_start();
			clk::gui::draw();
			ImGui::Render();
			profiler_draw.record_sample_end();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			profiler_swap.record_sample_start();
			glfwSwapBuffers(window);
			profiler_swap.record_sample_end();
		}

		ImPlot::DestroyContext();

		ImNodes::DestroyContext();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}
	catch(...)
	{
		return 1;
	}
	return 0;
}

// NOLINTNEXTLINE
void gl_debug_output(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/, const GLchar* message,
	const void* /*userParam*/)
{
	std::cout << "-----------------------------------\n"
			  << "OpenGL Debug Message (" << id << "): \n"
			  << message << '\n';
	std::cout << "Source: ";
	switch(source)
	{
		case GL_DEBUG_SOURCE_API:
			std::cout << "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			std::cout << "Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			std::cout << "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			std::cout << "Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			std::cout << "Application";
			break;
		default:
			std::cout << "Other";
			break;
	}
	std::cout << "\nType: ";
	switch(type)
	{
		case GL_DEBUG_TYPE_ERROR:
			std::cout << "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			std::cout << "Deprecated Behaviour";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			std::cout << "Undefined Behaviour";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			std::cout << "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			std::cout << "Performance";
			break;
		case GL_DEBUG_TYPE_MARKER:
			std::cout << "Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			std::cout << "Push Group";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			std::cout << "Pop Group";
			break;
		default:
			std::cout << "Other";
			break;
	}
	std::cout << "\nSeverity: ";
	switch(severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout << "High";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cout << "Medium";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			std::cout << "Low";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			std::cout << "Notification";
			break;
		default:
			std::cout << "Other";
			break;
	}
	std::cout << '\n';
}
