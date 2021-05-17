#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace clk::algorithms
{
class random_color_source final : public clk::algorithm
{
public:
	random_color_source();

	auto get_name() const noexcept -> std::string const& override;

private:
	clk::output_port_of<clk::color_rgb> color{"Color"};

	void update() override;
};

} // namespace clk::algorithms