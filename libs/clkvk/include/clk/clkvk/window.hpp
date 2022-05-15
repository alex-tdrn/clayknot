#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
#include "clk/base/port.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string_view>

namespace clk::clkvk
{
class window final : public clk::algorithm_builder<window>
{
public:
	static constexpr std::string_view name = "clkvk::window";

	window();
	window(window const&) = delete;
	window(window&&) = delete;
	auto operator=(window const&) -> window& = delete;
	auto operator=(window&&) -> window& = delete;
	~window() override;

private:
	clk::input_of<std::string> _title{"title"};
	clk::input_of<int> _width{"width"};
	clk::input_of<int> _height{"height"};
	GLFWwindow* _glfw_window = nullptr;
	bool _updated_size_programmatically = false;

	static void window_resized_callback(GLFWwindow* glfw_window, int width, int height);

	void update() override;
};

inline window::window()
{
	register_port(_title);
	register_port(_width);
	register_port(_height);

	_title.default_port().data() = "Vulkan stuff";
	_width.default_port().data() = 640;
	_height.default_port().data() = 480;
}

inline window::~window()
{
	glfwDestroyWindow(_glfw_window);
}

inline void window::window_resized_callback(GLFWwindow* glfw_window, int width, int height)
{
	auto* window = static_cast<clk::clkvk::window*>(glfwGetWindowUserPointer(glfw_window));
	if(window->_updated_size_programmatically)
	{
		window->_updated_size_programmatically = false;
		return;
	}

	if(!window->_width.is_connected())
		window->_width.default_port().data() = width;

	if(!window->_height.is_connected())
		window->_height.default_port().data() = height;

	window->update();
}

inline void window::update()
{
	_updated_size_programmatically = true;
	if(_glfw_window == nullptr)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		_glfw_window = glfwCreateWindow(*_width, *_height, _title->c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(_glfw_window, this);
		// glfwSetWindowSizeCallback(_glfw_window, window_resized_callback);
	}
	glfwSetWindowSize(_glfw_window, *_width, *_height);
	glfwSetWindowTitle(_glfw_window, _title->c_str());
}
} // namespace clk::clkvk