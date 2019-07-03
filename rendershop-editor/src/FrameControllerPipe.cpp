#include "FrameControllerPipe.h"

void FrameControllerPipe::registerOutputEvents()
{
	AbstractSink::registerOutputEvents();
	AbstractSource::registerOutputEvents();
	registerOutputEvent(OutputEvents::NewFrame, "New Frame");
}

void FrameControllerPipe::update()
{
}

void FrameControllerPipe::newFrame()
{
	
	trigger(OutputEvents::NewFrame);
}
