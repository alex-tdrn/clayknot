#include "clk/algorithms/color.hpp"
#include "clk/algorithms/init.hpp"
#include "clk/base/algorithm_node.hpp"
#include "clk/base/graph.hpp"
#include "clk/gui/init.hpp"
#include "clk/gui/panel.hpp"
#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/viewer.hpp"
#include "clk/util/color_rgba.hpp"
#include "clk/util/profiler.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <chrono>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imnodes.h>
#include <implot.h>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/,
	const GLchar* message, const void* /*userParam*/);
void ImGuiCherryStyle();

auto main(int /*argc*/, char** /*argv*/) -> int
{
	try
	{
		if(glfwInit() == 0)
			return 1;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

		GLFWwindow* window = glfwCreateWindow(1920, 1080, "clayknot", nullptr, nullptr);
		if(window == nullptr)
			return 1;

		glfwMakeContextCurrent(window);

		if(gladLoadGL() == 0)
			return 1;
#ifdef _DEBUG
		GLint context_flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
		if((context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
#endif

		glfwSwapInterval(0);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 420");

		ImNodes::CreateContext();
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
		clk::gui::init();

		auto graph1 = []() -> clk::graph {
			auto random_color =
				std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::random_color>());
			auto decompose_color =
				std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::decompose_color>());
			auto value_to_color =
				std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::value_to_color>());
			auto mix_colors = std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::mix_colors>());

			clk::graph ret;

			ret.emplace_back(std::move(random_color));
			ret.emplace_back(std::move(decompose_color));
			ret.emplace_back(std::move(value_to_color));
			ret.emplace_back(std::move(mix_colors));
			return ret;
		}();

		clk::gui::panel::create_orphan(clk::gui::viewer::create(&graph1, "graph1 viewer"));
		clk::gui::panel::create_orphan(clk::gui::editor::create(&graph1, "graph1 editor"));

		clk::profiler profiler;
		auto profiler_panel = clk::gui::panel(clk::gui::viewer::create(&profiler, "Frametimes"));
		profiler_panel.set_title_bar_visibility(false);
		profiler_panel.set_resizability(clk::gui::panel::resizability::off);
		profiler_panel.set_docking(false);
		profiler_panel.set_movability(false);
		profiler_panel.set_interactivity(false);
		profiler_panel.set_opactiy(0.5f);

		while(glfwWindowShouldClose(window) == 0)
		{
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

			profiler.record_frame();

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

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/, const GLchar* message,
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
