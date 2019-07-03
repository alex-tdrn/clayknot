#include "rendershop/pipes/RandomColorSource.h"

void RandomColorSource::update()
{
	auto& color = getOutputData<OutputPorts::Color>();
	color = {rand() % 256 / 256.0, rand() % 256 / 256.0, rand() % 256 / 256.0};
}
