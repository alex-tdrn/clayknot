#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/base/output.hpp"
#include "clk/base/port.hpp"

#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace clk::clkvk
{
class required_windowing_extensions final : public clk::algorithm_builder<required_windowing_extensions>
{
public:
	static constexpr std::string_view name = "clkvk::required_windowing_extensions";

	required_windowing_extensions()
	{
		register_port(_extensions);
	}

	required_windowing_extensions(required_windowing_extensions const&) = delete;
	required_windowing_extensions(required_windowing_extensions&&) = delete;
	auto operator=(required_windowing_extensions const&) -> required_windowing_extensions& = delete;
	auto operator=(required_windowing_extensions&&) -> required_windowing_extensions& = delete;
	~required_windowing_extensions() override = default;

private:
	clk::output_of<std::vector<std::string>> _extensions{"Extensions"};

	void update() override
	{
		uint32_t count = 0;
		const char** extensions = glfwGetRequiredInstanceExtensions(&count);
		_extensions->clear();
		_extensions->reserve(count);
		for(uint32_t extension_index = 0; extension_index < count; extension_index++)
		{
			_extensions->push_back(std::string(extensions[extension_index]));
		}
	}
};

} // namespace clk::clkvk