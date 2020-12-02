#include "rsp/nodes/ValueToColor.h"

namespace rsp::nodes
{
ValueToColor::ValueToColor()
{
	registerPort(value);
	registerPort(color);
}

auto ValueToColor::getName() const -> std::string const&
{
	static std::string name = "Value To Color";
	return name;
}

void ValueToColor::update()
{
	*color = rsp::ColorRGB{*value};
}
} // namespace rsp::nodes