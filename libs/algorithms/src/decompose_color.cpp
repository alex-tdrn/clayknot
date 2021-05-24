#include "clk/algorithms/decompose_color.hpp"

namespace clk::algorithms
{
decompose_color::decompose_color()
{
	register_port(_color);
	register_port(_red_component);
	register_port(_green_component);
	register_port(_blue_component);
}

auto decompose_color::get_name() const noexcept -> std::string const&
{
	static std::string name = "Decompose Colors";
	return name;
}

void decompose_color::update()
{
	*_red_component = _color->r() * 100;
	*_green_component = _color->g() * 100;
	*_blue_component = _color->b() * 100;
}
} // namespace clk::algorithms