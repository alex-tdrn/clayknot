#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace clk::algorithms
{
class value_to_color final : public clk::algorithm
{
public:
	value_to_color();

	auto name() const noexcept -> std::string const& override;

private:
	clk::input_of<float> _value{"Value"};
	clk::output_of<clk::color_rgb> _color{"Color"};

	void update() override;
};

} // namespace clk::algorithms