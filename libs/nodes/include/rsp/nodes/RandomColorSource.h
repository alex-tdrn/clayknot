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
private:
	OutputPort<ColorRGB> color{"Color"};

public:
	RandomColorSource()
	{
		registerPort(color);
	}

private:
	void update() override;

public:
	auto getName() const -> std::string const& override
	{
		static std::string name = "Random Color Source";
		return name;
	}
};

} // namespace rsp::nodes