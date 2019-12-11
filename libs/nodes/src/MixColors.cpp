#include "rsp/nodes/MixColors.h"

void rsp::nodes::MixColors::update()
{
	auto& factor = getInputData<InputPorts::Factor>();
	auto& colorA = getInputData<InputPorts::ColorA>();
	auto& colorB = getInputData<InputPorts::ColorB>();
	auto& mixedColor = getOutputData<OutputPorts::MixedColor>();

	mixedColor = factor * colorA + (1 - factor) * colorB;
}
