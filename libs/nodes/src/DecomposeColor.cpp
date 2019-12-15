#include "rsp/nodes/DecomposeColor.h"

void rsp::nodes::DecomposeColor::run()
{
	auto& color = getInputData<InputPorts::Color>();
	auto& r = getOutputData<OutputPorts::R>();
	auto& g = getOutputData<OutputPorts::G>();
	auto& b = getOutputData<OutputPorts::B>();

	r = color.r();
	g = color.g();
	b = color.b();
}
