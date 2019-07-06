#include "renderdeck\ClearBackgroundSink.h"

#include <glad/glad.h>

void ClearBackgroundSink::update() const
{
	auto const& color = getInputPort<InputPort::Color>().getValueAndTimestamp().first;
	glClearColor(color.r, color.g, color.b, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}
