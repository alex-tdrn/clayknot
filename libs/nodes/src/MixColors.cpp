#include "rshp/nodes/MixColors.h"

void rshp::nodes::MixColors::update()
{
	auto& factor = getInputData<InputPorts::Factor>();
	auto& colorA = getInputData<InputPorts::ColorA>();
	auto& colorB = getInputData<InputPorts::ColorB>();
	auto& mixedColor = getOutputData<OutputPorts::MixedColor>();

	mixedColor = factor * colorA + (1 - factor) * colorB;
}
