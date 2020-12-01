#include "rsp/nodes/RandomColorSource.h"

#include <chrono>
#include <random>

void rsp::nodes::RandomColorSource::update()
{
	std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> dis(0, 1);

	*color = ColorRGB{dis(generator), dis(generator), dis(generator)};
}
