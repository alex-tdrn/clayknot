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

	auto get_name() const noexcept -> std::string const& override;

private:
	clk::input_port_of<float> value{"Value"};
	clk::output_port_of<clk::color_rgb> color{"Color"};

	void update() override;
};

} // namespace clk::algorithms