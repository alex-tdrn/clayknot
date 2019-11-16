#include "rshp/pipes/ValueToColor.h"

void ValueToColor::update()
{
	auto& value = getInputData<InputPorts::Value>();
	auto& color = getOutputData<OutputPorts::Color>();

	color = {value, value, value};
}
