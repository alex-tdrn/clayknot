#include "rsp/nodes/RandomColorSource.h"

#include <stdlib.h>

void rsp::nodes::RandomColorSource::update()
{
	*color = ColorRGB{rand() % 256 / 256.0f, rand() % 256 / 256.0f, rand() % 256 / 256.0f};
}
