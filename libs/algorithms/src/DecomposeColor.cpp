#include "rsp/algorithms/DecomposeColor.h"

namespace rsp::algorithms
{
DecomposeColor::DecomposeColor()
{
	registerPort(color);
	registerPort(redComponent);
	registerPort(greenComponent);
	registerPort(blueComponent);
}

auto DecomposeColor::getName() const noexcept -> std::string const&
{
	static std::string name = "Decompose Colors";
	return name;
}

void DecomposeColor::update()
{
	*redComponent = color->r() * 100;
	*greenComponent = color->g() * 100;
	*blueComponent = color->b() * 100;
}
} // namespace rsp::algorithms