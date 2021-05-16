#pragma once

#include "rsp/base/algorithm.hpp"
#include "rsp/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace rsp::algorithms
{
class random_color_source final : public rsp::algorithm
{
public:
	random_color_source();

	auto get_name() const noexcept -> std::string const& override;

private:
	rsp::output_port_of<rsp::color_rgb> color{"Color"};

	void update() override;
};

} // namespace rsp::algorithms