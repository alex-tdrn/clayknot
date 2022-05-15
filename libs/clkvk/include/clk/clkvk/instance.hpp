#pragma once

#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
#include <clk/base/algorithm.hpp>
#include <vulkan/vulkan.hpp>

namespace clk::clkvk
{
class instance final : public clk::algorithm_builder<instance>
{
public:
	static constexpr std::string_view name = "clkvk::instance";

	instance();
	instance(instance const&) = delete;
	instance(instance&&) = delete;
	auto operator=(instance const&) -> instance& = delete;
	auto operator=(instance&&) -> instance& = delete;
	~instance() override = default;

private:
	void update() override;

	clk::input_of<vk::ApplicationInfo> _application_info;
	clk::output_of<vk::UniqueInstance> _instance;
};

inline instance::instance()
{
	register_port(_application_info);

	register_port(_instance);
}

inline void instance::update()
{
	// TODO
}

} // namespace clk::clkvk