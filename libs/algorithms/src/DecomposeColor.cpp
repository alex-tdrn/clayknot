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
	*redComponent = color->r();
	*greenComponent = color->g();
	*blueComponent = color->b();
}
} // namespace rsp::algorithms