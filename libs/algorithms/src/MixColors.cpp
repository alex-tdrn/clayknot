#include "rsp/algorithms/MixColors.h"

namespace rsp::algorithms
{
MixColors::MixColors()
{
	registerPort(factor);
	registerPort(colorA);
	registerPort(colorB);
	registerPort(mixedColor);
}

auto MixColors::getName() const noexcept -> std::string const&
{
	static std::string name = "Mix Colors";
	return name;
}

void MixColors::update()
{
	float f = factor.get() / 100.0f;
	*mixedColor = f * colorA.get() + (1 - f) * colorB.get();
}
} // namespace rsp::algorithms