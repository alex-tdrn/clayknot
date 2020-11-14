#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/util/ColorRGB.hpp"


#include <array>

namespace rsp::nodes
{
class MixColors final : public Node
{
private:
	InputPort<float> factor{"Factor"};
	InputPort<ColorRGB> colorA{"Color A"};
	InputPort<ColorRGB> colorB{"Color B"};
	OutputPort<ColorRGB> mixedColor{"Mixed Color"};

public:
	MixColors()
	{
		registerPort(factor);
		registerPort(colorA);
		registerPort(colorB);
		registerPort(mixedColor);
	}

private:
	void update() override;

public:
	std::string const& getName() const override
	{
		static std::string name = "Mix Colors";
		return name;
	}
};

} // namespace rsp::nodes