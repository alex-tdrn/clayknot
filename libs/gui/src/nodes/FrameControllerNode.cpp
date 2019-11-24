#include "rshp/gui/nodes/FrameControllerNode.h"

namespace rshp::gui
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
	
}