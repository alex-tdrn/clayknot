#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/util/ColorRGB.hpp"

#include <array>

namespace rsp::nodes
{
class ClearBackgroundSink final : public Node
{
private:
	InputPort<ColorRGB> clearColor{"Color"};

public:
	ClearBackgroundSink()
	{
		registerPort(clearColor);
	}

protected:
	void update() override;

public:
	std::string const& getName() const override
	{
		static std::string name = "Clear Background Sink";
		return name;
	}
};

} // namespace rsp::nodes