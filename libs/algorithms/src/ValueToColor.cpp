#include "rsp/algorithms/ValueToColor.h"

namespace rsp::nodes
{
ValueToColor::ValueToColor()
{
	registerPort(value);
	registerPort(color);
}

auto ValueToColor::getName() const noexcept -> std::string const&
{
	static std::string name = "Value To Color";
	return name;
}

void ValueToColor::update()
{
	*color = rsp::ColorRGB{*value};
}
} // namespace rsp::nodes