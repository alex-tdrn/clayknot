#include "clk/algorithms/value_to_color.hpp"

namespace clk::algorithms
{
value_to_color::value_to_color()
{
	register_port(_value);
	register_port(_color);
}

auto value_to_color::name() const noexcept -> std::string const&
{
	static std::string name = "Value To Color";
	return name;
}

void value_to_color::update()
{
	*_color = clk::color_rgb{*_value / 100.0f};
}
} // namespace clk::algorithms