#include "clk/algorithms/boolean.hpp"
#include "clk/algorithms/color.hpp"
#include "clk/algorithms/init.hpp"
#include "clk/algorithms/text.hpp"
#include "clk/base/algorithm.hpp"
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

#include "clk/clkvk/application_info.hpp"
#include "clk/clkvk/init.hpp"
#include "clk/clkvk/instance.hpp"
#include "clk/clkvk/required_windowing_extensions.hpp"
#include "clk/clkvk/window.hpp"

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
		clk::clkvk::init();
		auto widget_factory = clk::gui::create_default_factory();

		auto graph1 = clk::graph{};
		graph1.add_node(std::make_unique<clk::algorithm_node>(std::make_unique<clk::clkvk::application_info>()));
		graph1.add_node(std::make_unique<clk::algorithm_node>(std::make_unique<clk::clkvk::instance>()));
		graph1.add_node(
			std::make_unique<clk::algorithm_node>(std::make_unique<clk::clkvk::required_windowing_extensions>()));

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

		while(glfwWindowShouldClose(window) == 0)
		{
			profiler_frame.record_sample_end();
			profiler_frame.record_sample_start();

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

			clk::gui::draw();
			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			glfwSwapBuffers(window);
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
