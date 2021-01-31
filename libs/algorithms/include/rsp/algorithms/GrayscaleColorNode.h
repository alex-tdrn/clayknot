#pragma once

#include "rsp/base/Algorithm.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <string>
#include <unordered_set>

namespace rsp::algorithms
{
class GrayscaleColorNode final : public Algorithm
{
public:
	GrayscaleColorNode();
	auto getName() const noexcept -> std::string const& override;

private:
	InputPortOf<ColorRGB> inputColor{"Color"};
	OutputPortOf<ColorRGB> outputColor{"Color"};

	void update() override;
};

} // namespace rsp::algorithms