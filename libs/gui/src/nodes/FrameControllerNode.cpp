#include "rsp/gui/nodes/FrameControllerNode.h"

namespace rsp::gui
{
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

} // namespace rsp::gui