#include "rshp/pipes/ValueToColor.h"

void rshp::pipes::ValueToColor::update()
{
	auto& value = getInputData<InputPorts::Value>();
	auto& color = getOutputData<OutputPorts::Color>();

	color = {value, value, value};
}
