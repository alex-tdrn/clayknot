#include "rsp/algorithms/ValueToColor.h"

namespace rsp::algorithms
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
	*color = rsp::ColorRGB{*value / 100.0f};
}
} // namespace rsp::algorithms