#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace clk::algorithms
{
class decompose_color final : public clk::algorithm
{
public:
	decompose_color();
	auto get_name() const noexcept -> std::string const& final;

private:
	clk::input_port_of<clk::color_rgb> color{"Color"};
	clk::output_port_of<float> red_component{"R"};
	clk::output_port_of<float> green_component{"G"};
	clk::output_port_of<float> blue_component{"B"};

	void update() final;
};

} // namespace clk::algorithms