#include "InputPin.h"
#include "OutputPin.h"
#include "Stylesheet.hpp"

InputPin::InputPin(AbstractPort* port)
	: AbstractPin(port), linkID(uniqueID())
{
	auto connections = port->getConnections();
	assert(connections.size() <= 1);
	for(auto connection : connections)
		if(auto pin = getPinForPort(connection); pin != nullptr)
			connect(pin);
}

ImVec2 InputPin::calculateAnchorPosition() const
{
	auto currentStyle = Stylesheet::getCurrentSheet();
	auto min = ImGui::GetItemRectMin();
	auto max = ImGui::GetItemRectMax();
	auto h = max.y - min.y;
	auto x = min.x - anchorOffset.get(currentStyle.animatedAnchorOffset,
		currentStyle.anchorOffset, currentStyle.animatedAnchorOffsetDuration, AnimationCurve::spring);
	auto y = min.y + h / 2;
	return {x, y};
}

bool InputPin::canConnect(AbstractPin* outputPin)
{
	if(!dynamic_cast<OutputPin*>(outputPin))
		return false;
	return port->canConnect(outputPin->getPort());
}

void InputPin::connect(AbstractPin* outputPin)
{
	assert(dynamic_cast<OutputPin*>(outputPin));
	assert(canConnect(outputPin));

	port->connect(outputPin->getPort());
	connection = dynamic_cast<OutputPin*>(outputPin);
}
