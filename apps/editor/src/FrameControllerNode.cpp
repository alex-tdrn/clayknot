#include "FrameControllerNode.h"

void FrameControllerNode::registerOutputEvents()
{
	AbstractSink::registerOutputEvents();
	AbstractSource::registerOutputEvents();
	registerOutputEvent(OutputEvents::NewFrame, "New Frame");
}

void FrameControllerNode::update()
{
}

void FrameControllerNode::newFrame()
{
	
	trigger(OutputEvents::NewFrame);
}
