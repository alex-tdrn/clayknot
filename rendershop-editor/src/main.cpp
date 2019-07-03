#include "rendershop/pipes/RandomColorSource.h"
#include "rendershop/pipes/ClearBackgroundSink.h"
#include "rendershop/pipes/GrayscaleColorPipe.h"
#include "rendershop/pipes/MixColors.h"
#include "rendershop/pipes/Timer.hpp"
#include "rendershop/pipes/DecomposeColor.h"
#include "rendershop/pipes/ValueToColor.h"
#include "NodeCanvas.h"
#include "RootWindow.h"
#include "FrameControllerPipe.h"
#include "Stylesheet.hpp"
#include "StylesheetWindow.h"

#include <imgui.h>
#include <imgui_node_editor.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <vector>


void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void ImGuiCherryStyle();

int main(int argc, char** argv)
{
	if(!glfwInit())
		throw "Failed to initialize GLFW!";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
	
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "rendershop", nullptr, nullptr);
	if(!window)
		throw "Failed to create GLFW window\n";

	glfwMakeContextCurrent(window);

	if(!gladLoadGL())
		throw "Failed to initialize GLAD\n";

#ifdef _DEBUG
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if(flags & GL_CONTEXT_FLAG_DEBUG_BIT)
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

	ImGuiCherryStyle();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420");

	
	using namespace std::chrono_literals;
	std::vector<std::unique_ptr<AbstractPipe>> pipes;
	
	std::unique_ptr<AbstractPipe> source = std::make_unique<RandomColorSource>();
	std::unique_ptr<AbstractPipe> sink = std::make_unique<ClearBackgroundSink>();
	std::unique_ptr<AbstractPipe> timer = std::make_unique<Timer>();
	auto tmpFrameController = std::make_unique<FrameControllerPipe>();
	FrameControllerPipe* frameController = tmpFrameController.get();

	timer->getOutputEventPort(Timer::OutputEvents::Timeout).connect(&sink->getInputEventPort(ClearBackgroundSink::InputEvents::Run));
	timer->getOutputEventPort(Timer::OutputEvents::Timeout).connect(&source->getInputEventPort(RandomColorSource::InputEvents::QueueUpdate));
	timer->getInputEventPort(Timer::InputEvents::Poll).connect(&frameController->getOutputEventPort(FrameControllerPipe::OutputEvents::NewFrame));
	/*dynamic_cast<RandomColorSource*>(source.get())->getOutputDataPort<RandomColorSource::OutputPorts::Color>()
		.connect(&dynamic_cast<ClearBackgroundSink*>(sink.get())->getInputDataPort<ClearBackgroundSink::InputPorts::Color>());*/
	
	//TODO
	pipes.push_back(std::move(source));
	pipes.push_back(std::move(sink));
	pipes.push_back(std::move(timer));
	pipes.push_back(std::move(tmpFrameController));
	

	Stylesheet::addSheet(Stylesheet());

	RootWindow rootWindow;
	NodeCanvas* canvas = rootWindow.addChild(std::make_unique<NodeCanvas>());
	canvas->setStore(&pipes);
	rootWindow.addChild(std::make_unique<StylesheetWindow>());

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		frameController->newFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		int wind_x, wind_y;
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwGetWindowPos(window, &wind_x, &wind_y);

		rootWindow.draw();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void ImGuiCherryStyle() 
{
	// cherry colors, 3 intensities
#define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
#define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
#define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
// backgrounds (@todo: complete with BG_MED, BG_LOW)
#define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
// text
#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = TEXT(0.78f);
	style.Colors[ImGuiCol_TextDisabled] = TEXT(0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = BG(0.58f);
	style.Colors[ImGuiCol_PopupBg] = BG(0.9f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = BG(1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = MED(1.00f);
	style.Colors[ImGuiCol_TitleBg] = LOW(1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = HI(1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = BG(0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = BG(1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
	style.Colors[ImGuiCol_ButtonActive] = MED(1.00f);
	style.Colors[ImGuiCol_Header] = MED(0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
	style.Colors[ImGuiCol_HeaderActive] = HI(1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = MED(1.00f);
	style.Colors[ImGuiCol_PlotLines] = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = MED(1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
	// [...]
	style.Colors[ImGuiCol_ModalWindowDarkening] = BG(0.73f);

	style.WindowPadding = ImVec2(6, 4);
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2(5, 2);
	style.FrameRounding = 3.0f;
	style.ItemSpacing = ImVec2(7, 1);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;

	style.WindowTitleAlign.x = 0.50f;

	style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
	style.FrameBorderSize = 0.0f;
	style.WindowBorderSize = 1.0f;
}

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "-----------------------------------\n"
		<< "OpenGL Debug Message (" << id << "): \n" << message << '\n';
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
	}
	std::cout << '\n';
}
