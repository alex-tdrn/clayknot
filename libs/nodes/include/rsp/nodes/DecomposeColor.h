#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <array>

namespace rsp::nodes
{
class DecomposeColor final : public Node
{
private:
	InputPort<ColorRGB> color{"Color"};
	OutputPort<float> redComponent{"R"};
	OutputPort<float> greenComponent{"R"};
	OutputPort<float> blueComponent{"B"};

public:
	DecomposeColor()
	{
		registerPort(color);
		registerPort(redComponent);
		registerPort(greenComponent);
		registerPort(blueComponent);
	}

protected:
	void update() override;

public:
	std::string const& getName() const override
	{
		static std::string name = "Decompose Colors";
		return name;
	}
};

} // namespace rsp::nodes