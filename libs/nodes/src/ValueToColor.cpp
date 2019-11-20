#include "rshp/nodes/ValueToColor.h"

void rshp::nodes::ValueToColor::update()
{
	auto& value = getInputData<InputPorts::Value>();
	auto& color = getOutputData<OutputPorts::Color>();

	color = {value, value, value};
}
