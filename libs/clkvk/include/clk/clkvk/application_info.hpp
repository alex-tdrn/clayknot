#pragma once

#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
#include "clk/base/port.hpp"

#include <clk/base/algorithm.hpp>
#include <string_view>
#include <vulkan/vulkan.hpp>

namespace clk::clkvk
{
class application_info final : public clk::algorithm_builder<application_info>
{
public:
	static constexpr std::string_view name = "clkvk::application_info";

	application_info();
	application_info(application_info const&) = delete;
	application_info(application_info&&) = delete;
	auto operator=(application_info const&) -> application_info& = delete;
	auto operator=(application_info&&) -> application_info& = delete;
	~application_info() override = default;

private:
	void update() override;

	clk::input_of<std::string> _application_name{"Application Name"};
	clk::input_of<unsigned int> _application_major_version{"Application Major Version"};
	clk::input_of<unsigned int> _application_minor_version{"Application Minor Version"};
	clk::input_of<unsigned int> _application_patch_version{"Application Patch Version"};

	clk::input_of<std::string> _engine_name{"Engine Name"};
	clk::input_of<unsigned int> _engine_major_version{"Engine Major Version"};
	clk::input_of<unsigned int> _engine_minor_version{"Engine Minor Version"};
	clk::input_of<unsigned int> _engine_patch_version{"Engine Patch Version"};

	clk::input_of<unsigned int> _api_major_version{"API Major Version"};
	clk::input_of<unsigned int> _api_minor_version{"API Minor Version"};
	clk::input_of<unsigned int> _api_patch_version{"API Patch Version"};

	clk::output_of<vk::ApplicationInfo> _out{"Info"};
};

inline application_info::application_info()
{
	register_port(_application_name);
	register_port(_application_major_version);
	register_port(_application_minor_version);
	register_port(_application_patch_version);

	register_port(_engine_major_version);
	register_port(_engine_minor_version);
	register_port(_engine_patch_version);

	register_port(_api_major_version);
	register_port(_api_minor_version);
	register_port(_api_patch_version);

	register_port(_engine_name);

	register_port(_out);

	_application_name.default_port().data() = "";
	_application_major_version.default_port().data() = 0;
	_application_minor_version.default_port().data() = 0;
	_application_patch_version.default_port().data() = 0;

	_engine_name.default_port().data() = "";
	_engine_major_version.default_port().data() = 0;
	_engine_minor_version.default_port().data() = 0;
	_engine_patch_version.default_port().data() = 0;

	_api_major_version.default_port().data() = 1;
	_api_minor_version.default_port().data() = 2;
	_api_patch_version.default_port().data() = 0;
}

inline void application_info::update()
{
	*_out = vk::ApplicationInfo{};

	_out->setPApplicationName(_application_name->c_str());
	_out->setApplicationVersion(
		VK_MAKE_VERSION(*_application_major_version, *_application_minor_version, *_application_patch_version));

	_out->setPEngineName(_engine_name->c_str());
	_out->setEngineVersion(VK_MAKE_VERSION(*_engine_major_version, *_engine_minor_version, *_engine_patch_version));

	_out->setApiVersion(VK_MAKE_VERSION(*_api_major_version, *_api_minor_version, *_api_patch_version));
}

} // namespace clk::clkvk