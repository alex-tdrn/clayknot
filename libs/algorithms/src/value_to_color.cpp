#include "clk/algorithms/value_to_color.h"

namespace clk::algorithms
{
value_to_color::value_to_color()
{
	register_port(value);
	register_port(color);
}

auto value_to_color::get_name() const noexcept -> std::string const&
{
	static std::string name = "Value To Color";
	return name;
}

void value_to_color::update()
{
	*color = clk::color_rgb{*value / 100.0f};
}
} // namespace clk::algorithms