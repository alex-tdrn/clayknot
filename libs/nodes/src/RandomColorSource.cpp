#include "rshp/nodes/RandomColorSource.h"

void rshp::nodes::RandomColorSource::update()
{
	auto& color = getOutputData<OutputPorts::Color>();
	color = {rand() % 256 / 256.0, rand() % 256 / 256.0, rand() % 256 / 256.0};
}
