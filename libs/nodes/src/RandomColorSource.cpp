#include "rshp/nodes/RandomColorSource.h"

void rshp::nodes::RandomColorSource::update()
{
	auto& color = getOutputData<OutputPorts::Color>();
	color = {rand() % 256 / 256.0f, rand() % 256 / 256.0f, rand() % 256 / 256.0f};
}
