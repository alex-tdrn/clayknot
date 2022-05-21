#pragma once

#include "clk/base/output.hpp"
#include <clk/base/algorithm.hpp>
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace clk::clkvk
{
class enumerate_instance_extensions final : public clk::algorithm_builder<enumerate_instance_extensions>
{
public:
	static constexpr std::string_view name = "clkvk::enumerate_instance_extensions";

	enumerate_instance_extensions()
	{
		register_port(_extensions);
	}

	enumerate_instance_extensions(enumerate_instance_extensions const&) = delete;
	enumerate_instance_extensions(enumerate_instance_extensions&&) = delete;
	auto operator=(enumerate_instance_extensions const&) -> enumerate_instance_extensions& = delete;
	auto operator=(enumerate_instance_extensions&&) -> enumerate_instance_extensions& = delete;
	~enumerate_instance_extensions() override = default;

private:
	void update() override
	{
		_extensions->clear();
		for(auto const& extension : vk::enumerateInstanceExtensionProperties())
		{
			std::string version = std::to_string(VK_VERSION_MAJOR(extension.specVersion)) + "." +
								  std::to_string(VK_VERSION_MINOR(extension.specVersion)) + "." +
								  std::to_string(VK_VERSION_PATCH(extension.specVersion));

			_extensions->push_back(std::string(extension.extensionName) + "\nv" + version);
		}
	}

	clk::output_of<std::vector<std::string>> _extensions{"Available instance extensions"};
};
} // namespace clk::clkvk