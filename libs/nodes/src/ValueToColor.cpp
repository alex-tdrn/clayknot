#include "rsp/nodes/ValueToColor.h"

void rsp::nodes::ValueToColor::update()
{
	auto& value = getInputData<InputPorts::Value>();
	auto& color = getOutputData<OutputPorts::Color>();

	color = rsp::ColorRGB{value};
}
