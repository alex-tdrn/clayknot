#include "rshp/pipes/RandomColorSource.h"

void rshp::pipes::RandomColorSource::update()
{
	auto& color = getOutputData<OutputPorts::Color>();
	color = {rand() % 256 / 256.0, rand() % 256 / 256.0, rand() % 256 / 256.0};
}
