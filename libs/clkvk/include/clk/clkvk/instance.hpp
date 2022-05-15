#pragma once

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

	vk::UniqueInstance _instance;
};

inline instance::instance()
{
}

inline void instance::update()
{
	if(!_instance)
	{
		auto app_info = vk::ApplicationInfo().setPApplicationName("Clayknot").setApplicationVersion(VK_API_VERSION_1_2);
	}
}

} // namespace clk::clkvk