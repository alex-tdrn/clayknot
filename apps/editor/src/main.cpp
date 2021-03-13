#include "rsp/algorithms/DecomposeColor.h"
#include "rsp/algorithms/GrayscaleColorNode.h"
#include "rsp/algorithms/MixColors.h"
#include "rsp/algorithms/RandomColorSource.h"
#include "rsp/algorithms/ValueToColor.h"
#include "rsp/base/Graph.hpp"
#include "rsp/base/Node.hpp"
#include "rsp/gui/Init.h"
#include "rsp/gui/Panel.h"
#include "rsp/gui/widgets/Editor.hpp"
#include "rsp/gui/widgets/Viewer.hpp"
#include "rsp/util/ColorRGBA.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <chrono>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imnodes.h>
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

		GLFWwindow* window = glfwCreateWindow(1920, 1080, "rendershop", nullptr, nullptr);
		if(window == nullptr)
			return 1;

		glfwMakeContextCurrent(window);

		if(gladLoadGL() == 0)
			return 1;
#ifdef _DEBUG
		GLint flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if((flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
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

		imnodes::Initialize();
		auto& style = imnodes::GetStyle();
		// TODO these colors need to come form a stylesheet
		style.colors[imnodes::ColorStyle_LinkHovered] = rsp::ColorRGBA{1.0f}.packed();
		style.colors[imnodes::ColorStyle_LinkSelected] = rsp::ColorRGBA{1.0f}.packed();
		style.colors[imnodes::ColorStyle_Link] = rsp::ColorRGBA{0.0f}.packed();
		style.colors[imnodes::ColorStyle_PinHovered] = rsp::ColorRGBA{1.0f}.packed();

		style.colors[imnodes::ColorStyle_TitleBarHovered] = style.colors[imnodes::ColorStyle_TitleBar];
		style.colors[imnodes::ColorStyle_TitleBarSelected] = style.colors[imnodes::ColorStyle_TitleBar];
		style.colors[imnodes::ColorStyle_NodeBackgroundHovered] = style.colors[imnodes::ColorStyle_NodeBackground];
		style.colors[imnodes::ColorStyle_NodeBackgroundSelected] = style.colors[imnodes::ColorStyle_NodeBackground];

		rsp::gui::init();

		auto graph1 = []() -> rsp::Graph {
			auto randomColor = std::make_unique<rsp::Node>(std::make_unique<rsp::algorithms::RandomColorSource>());
			auto decomposeColor = std::make_unique<rsp::Node>(std::make_unique<rsp::algorithms::DecomposeColor>());
			auto valueToColor = std::make_unique<rsp::Node>(std::make_unique<rsp::algorithms::ValueToColor>());
			auto mixColors = std::make_unique<rsp::Node>(std::make_unique<rsp::algorithms::MixColors>());

			rsp::Graph ret;

			ret.emplace_back(std::move(randomColor));
			ret.emplace_back(std::move(decomposeColor));
			ret.emplace_back(std::move(valueToColor));
			ret.emplace_back(std::move(mixColors));
			return ret;
		}();

		std::vector<rsp::gui::Panel> panels;
		auto testPanel = rsp::gui::Panel("test");
		auto graphViewer = rsp::gui::Panel("graph viewer");
		auto graphEditor = rsp::gui::Panel("graph editor");
		auto styleEditor = rsp::gui::Panel("style editor");

		auto testColor = rsp::ColorRGB();

		testPanel.addWidget(rsp::gui::Viewer::create(&testColor, "test color viewer"));
		testPanel.addWidget(rsp::gui::Editor::create(&testColor, "test color editor"));

		graphViewer.addWidget(rsp::gui::Viewer::create(&graph1, "graph1 viewer"));
		graphEditor.addWidget(rsp::gui::Editor::create(&graph1, "graph1 editor"));

		panels.emplace_back(rsp::gui::Panel(testPanel));
		panels.push_back(std::move(testPanel));
		panels.push_back(std::move(graphViewer));
		panels.push_back(std::move(graphEditor));
		panels.push_back(std::move(styleEditor));

		while(glfwWindowShouldClose(window) == 0)
		{
			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			int wind_x = 0;
			int wind_y = 0;
			int display_w = 0;
			int display_h = 0;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwGetWindowPos(window, &wind_x, &wind_y);

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
			flags |= ImGuiWindowFlags_NoDocking;
			flags |= ImGuiWindowFlags_NoTitleBar;
			flags |= ImGuiWindowFlags_NoCollapse;
			flags |= ImGuiWindowFlags_NoResize;
			flags |= ImGuiWindowFlags_NoMove;
			flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			flags |= ImGuiWindowFlags_NoNavFocus;
			ImGui::Begin("Main Window", nullptr, flags);
			ImGui::PopStyleVar(3);

			ImGuiID dockspace_id = ImGui::GetID("Main Window Dockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

			if(ImGui::BeginMenuBar())
			{
				if(ImGui::BeginMenu("View"))
				{
					for(auto& panel : panels)
						if(ImGui::MenuItem(panel.getTitle().c_str(), "", panel.isVisible()))
							panel.toggleVisibility();
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::End();
			for(auto& panel : panels)
				panel.draw();

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

		imnodes::Shutdown();

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
