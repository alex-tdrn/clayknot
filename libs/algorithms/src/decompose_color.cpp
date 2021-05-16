#include "rsp/algorithms/decompose_color.h"

namespace rsp::algorithms
{
decompose_color::decompose_color()
{
	register_port(color);
	register_port(red_component);
	register_port(green_component);
	register_port(blue_component);
}

auto decompose_color::get_name() const noexcept -> std::string const&
{
	static std::string name = "Decompose Colors";
	return name;
}

void decompose_color::update()
{
	*red_component = color->r() * 100;
	*green_component = color->g() * 100;
	*blue_component = color->b() * 100;
}
} // namespace rsp::algorithms