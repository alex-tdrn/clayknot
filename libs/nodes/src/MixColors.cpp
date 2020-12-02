#include "rsp/nodes/MixColors.h"

namespace rsp::nodes
{
MixColors::MixColors()
{
	registerPort(factor);
	registerPort(colorA);
	registerPort(colorB);
	registerPort(mixedColor);
}

auto MixColors::getName() const -> std::string const&
{
	static std::string name = "Mix Colors";
	return name;
}

void MixColors::update()
{
	*mixedColor = factor.get() * colorA.get() + (1 - factor.get()) * colorB.get();
}
} // namespace rsp::nodes