#include "rsp/nodes/RandomColorSource.h"

void rsp::nodes::RandomColorSource::run()
{
	auto& color = getOutputData<OutputPorts::Color>();
	color = {rand() % 256 / 256.0f, rand() % 256 / 256.0f, rand() % 256 / 256.0f};
}
