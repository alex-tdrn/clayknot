#pragma once

#include "rsp/base/algorithm.hpp"
#include "rsp/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace rsp::algorithms
{
class grayscale_color_node final : public rsp::algorithm
{
public:
	grayscale_color_node();
	auto get_name() const noexcept -> std::string const& override;

private:
	rsp::input_port_of<rsp::color_rgb> input_color{"Color"};
	rsp::output_port_of<rsp::color_rgb> output_color{"Color"};

	void update() override;
};

} // namespace rsp::algorithms