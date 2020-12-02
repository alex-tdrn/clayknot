#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/base/Port.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <string>
#include <unordered_set>

namespace rsp::nodes
{
class DecomposeColor final : public Node
{
public:
	DecomposeColor();
	auto getName() const -> std::string const& override;

private:
	InputPort<ColorRGB> color{"Color"};
	OutputPort<float> redComponent{"R"};
	OutputPort<float> greenComponent{"R"};
	OutputPort<float> blueComponent{"B"};

	void update() override;
};

} // namespace rsp::nodes