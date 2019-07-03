#include "rendershop/pipes/DecomposeColor.h"

void DecomposeColor::update()
{
	auto& color = getInputData<InputPorts::Color>();
	auto& r = getOutputData<OutputPorts::R>();
	auto& g = getOutputData<OutputPorts::G>();
	auto& b = getOutputData<OutputPorts::B>();

	r = color.r;
	g = color.g;
	b = color.b;
}
