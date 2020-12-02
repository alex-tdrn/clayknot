#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/base/Port.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <string>
#include <unordered_set>

namespace rsp::nodes
{
class RandomColorSource final : public Node
{
public:
	RandomColorSource();

	auto getName() const -> std::string const& override;

private:
	OutputPort<ColorRGB> color{"Color"};

	void update() override;
};

} // namespace rsp::nodes