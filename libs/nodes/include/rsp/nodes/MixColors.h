#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/base/Port.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <string>
#include <unordered_set>

namespace rsp::nodes
{
class MixColors final : public Node
{
public:
	MixColors();

	auto getName() const -> std::string const& override;

private:
	InputPortOf<float> factor{"Factor"};
	InputPortOf<ColorRGB> colorA{"Color A"};
	InputPortOf<ColorRGB> colorB{"Color B"};
	OutputPortOf<ColorRGB> mixedColor{"Mixed Color"};

	void update() override;
};

} // namespace rsp::nodes