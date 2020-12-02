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
	InputPort<float> factor{"Factor"};
	InputPort<ColorRGB> colorA{"Color A"};
	InputPort<ColorRGB> colorB{"Color B"};
	OutputPort<ColorRGB> mixedColor{"Mixed Color"};

	void update() override;
};

} // namespace rsp::nodes