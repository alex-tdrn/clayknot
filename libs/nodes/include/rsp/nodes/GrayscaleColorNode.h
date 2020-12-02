#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/base/Port.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <string>
#include <unordered_set>

namespace rsp::nodes
{
class GrayscaleColorNode final : public Node
{
public:
	GrayscaleColorNode();
	auto getName() const -> std::string const& override;

private:
	InputPort<ColorRGB> inputColor{"Color"};
	OutputPort<ColorRGB> outputColor{"Color"};

	void update() override;
};

} // namespace rsp::nodes