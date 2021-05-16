#pragma once

#include "rsp/base/algorithm.hpp"
#include "rsp/util/color_rgb.hpp"

#include <string>
#include <unordered_set>

namespace rsp::algorithms
{
class value_to_color final : public rsp::algorithm
{
public:
	value_to_color();

	auto get_name() const noexcept -> std::string const& override;

private:
	rsp::input_port_of<float> value{"Value"};
	rsp::output_port_of<rsp::color_rgb> color{"Color"};

	void update() override;
};

} // namespace rsp::algorithms