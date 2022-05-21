#pragma once

#include "clk/base/output.hpp"
#include <clk/base/algorithm.hpp>
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace clk::clkvk
{
class enumerate_instance_layers final : public clk::algorithm_builder<enumerate_instance_layers>
{
public:
	static constexpr std::string_view name = "clkvk::enumerate_instance_layers";

	enumerate_instance_layers()
	{
		register_port(_layers);
	}

	enumerate_instance_layers(enumerate_instance_layers const&) = delete;
	enumerate_instance_layers(enumerate_instance_layers&&) = delete;
	auto operator=(enumerate_instance_layers const&) -> enumerate_instance_layers& = delete;
	auto operator=(enumerate_instance_layers&&) -> enumerate_instance_layers& = delete;
	~enumerate_instance_layers() override = default;

private:
	void update() override
	{
		_layers->clear();
		for(auto const& layer : vk::enumerateInstanceLayerProperties())
		{
			_layers->push_back(layer.layerName);

			std::string spec_version = std::to_string(VK_VERSION_MAJOR(layer.specVersion)) + "." +
									   std::to_string(VK_VERSION_MINOR(layer.specVersion)) + "." +
									   std::to_string(VK_VERSION_PATCH(layer.specVersion));

			std::string implementation_version = std::to_string(VK_VERSION_MAJOR(layer.implementationVersion)) + "." +
												 std::to_string(VK_VERSION_MINOR(layer.implementationVersion)) + "." +
												 std::to_string(VK_VERSION_PATCH(layer.implementationVersion));
			std::string full_text = std::string(layer.layerName) + "\n";
			full_text += "spec_version:" + spec_version + "\n";
			full_text += "implementation_version:" + implementation_version + "\n";
			if(!layer.description.empty())
				full_text += std::string(layer.description) + "\n";

			_layers->push_back(std::move(full_text));
		}
	}

	clk::output_of<std::vector<std::string>> _layers{"Available instance layers"};
};
} // namespace clk::clkvk