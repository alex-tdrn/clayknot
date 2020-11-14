#include "rsp/nodes/ClearBackgroundSink.h"

#include <glad/glad.h>

void rsp::nodes::ClearBackgroundSink::update()
{
	glClearColor(clearColor->r(), clearColor->g(), clearColor->b(), 1);
	glClear(GL_COLOR_BUFFER_BIT);
}
