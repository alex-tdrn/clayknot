#pragma once

#include "rsp/base/Algorithm.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <string>
#include <unordered_set>

namespace rsp::algorithms
{
class MixColors final : public Algorithm
{
public:
	MixColors();

	auto getName() const noexcept -> std::string const& override;

private:
	InputPortOf<float> factor{"Factor"};
	InputPortOf<ColorRGB> colorA{"Color A"};
	InputPortOf<ColorRGB> colorB{"Color B"};
	OutputPortOf<ColorRGB> mixedColor{"Mixed Color"};

	void update() override;
};

} // namespace rsp::algorithms