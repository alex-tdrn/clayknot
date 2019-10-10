#include "renderdeck\ClearBackgroundSink.h"

#include <glad/glad.h>

void ClearBackgroundSink::update()
{
	auto const& color = getInputDataPort<InputPorts::Color>().getData();
	glClearColor(color.r, color.g, color.b, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}
