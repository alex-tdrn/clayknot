#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace clk::algorithms
{
class mix_colors final : public clk::algorithm
{
public:
	mix_colors();

	auto name() const noexcept -> std::string const& override;

private:
	clk::input_of<float> _factor{"Factor"};
	clk::input_of<clk::color_rgb> _color_a{"Color A"};
	clk::input_of<clk::color_rgb> _color_b{"Color B"};
	clk::output_of<clk::color_rgb> _mixed_color{"Mixed Color"};

	void update() override;
};

} // namespace clk::algorithms