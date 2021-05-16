#pragma once

#include "rsp/base/algorithm.hpp"
#include "rsp/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace rsp::algorithms
{
class decompose_color final : public rsp::algorithm
{
public:
	decompose_color();
	auto get_name() const noexcept -> std::string const& final;

private:
	rsp::input_port_of<rsp::color_rgb> color{"Color"};
	rsp::output_port_of<float> red_component{"R"};
	rsp::output_port_of<float> green_component{"G"};
	rsp::output_port_of<float> blue_component{"B"};

	void update() final;
};

} // namespace rsp::algorithms