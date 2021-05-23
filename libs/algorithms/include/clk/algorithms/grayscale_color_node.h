#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace clk::algorithms
{
class grayscale_color_node final : public clk::algorithm
{
public:
	grayscale_color_node();
	auto get_name() const noexcept -> std::string const& override;

private:
	clk::input_port_of<clk::color_rgb> _input_color{"Color"};
	clk::output_port_of<clk::color_rgb> _output_color{"Color"};

	void update() override;
};

} // namespace clk::algorithms