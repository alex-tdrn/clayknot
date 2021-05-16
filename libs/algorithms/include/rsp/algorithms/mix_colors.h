#pragma once

#include "rsp/base/algorithm.hpp"
#include "rsp/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace rsp::algorithms
{
class mix_colors final : public rsp::algorithm
{
public:
	mix_colors();

	auto get_name() const noexcept -> std::string const& override;

private:
	rsp::input_port_of<float> factor{"Factor"};
	rsp::input_port_of<rsp::color_rgb> color_a{"Color A"};
	rsp::input_port_of<rsp::color_rgb> color_b{"Color B"};
	rsp::output_port_of<rsp::color_rgb> mixed_color{"Mixed Color"};

	void update() override;
};

} // namespace rsp::algorithms