#include "rshp/gui/nodes/InputPort.h"
#include "rshp/gui/Stylesheet.hpp"
#include "rshp/gui/nodes/OutputPort.h"

namespace rshp::gui
{
InputPort::InputPort(rshp::base::Port* port) : AbstractPort(port), linkID(uniqueID())
{
	auto connections = port->getConnections();
	assert(connections.size() <= 1);
	for(auto connection : connections)
		if(auto pin = getPinForPort(connection); pin != nullptr)
			connect(pin);
}

ImVec2 InputPort::calculateAnchorPosition() const
{
	auto currentStyle = Stylesheet::getCurrentSheet();
	auto min = ImGui::GetItemRectMin();
	auto max = ImGui::GetItemRectMax();
	auto h = max.y - min.y;
	auto x = min.x - anchorOffset.get(currentStyle.animatedAnchorOffset, currentStyle.anchorOffset,
						 currentStyle.animatedAnchorOffsetDuration, AnimationCurve::spring);
	auto y = min.y + h / 2;
	return {x, y};
}

bool InputPort::canConnect(AbstractPort* outputPort)
{
	if(!dynamic_cast<OutputPort*>(outputPort))
		return false;
	return port->canConnect(outputPort->getPort());
}

void InputPort::connect(AbstractPort* outputPort)
{
	assert(dynamic_cast<OutputPort*>(outputPort));
	assert(canConnect(outputPort));

	port->connect(outputPort->getPort());
	connection = dynamic_cast<OutputPort*>(outputPort);
}

} // namespace rshp::gui