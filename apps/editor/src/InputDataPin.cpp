#include "InputDataPin.h"
#include "OutputPin.h"
#include "rshp/base/AbstractDataPort.hpp"
#include "ImGuiUtilities.hpp"
#include "Stylesheet.hpp"

InputDataPin::InputDataPin(AbstractDataPort* port)
	: InputPin(port), port(port)
{
}

void InputDataPin::draw()
{
	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Input);

	ImGui::Text(port->getName().c_str());

	dataRequested = port->getRequestCount() > dataRequests;
	dataRequests = port->getRequestCount();
	if(dataRequested && !port->isConnected())
		anchorOffset.play();

	auto anchorPosition = calculateAnchorPosition();
	auto anchorColor = ImGui::ColorFromHash(port->getDataTypeHash());

	if(port->isConnected())
		ImGui::DrawCircle(anchorPosition, 5, anchorColor);
	else
		ImGui::DrawCircle(anchorPosition, 5, {0, 0, 0, 1}, anchorColor);

	ax::NodeEditor::PinPivotRect(anchorPosition, anchorPosition);

	ax::NodeEditor::EndPin();
}

ImVec2 InputDataPin::calculateSize() const
{
	return ImGui::CalcTextSize(port->getName().c_str());
}

void InputDataPin::drawLink()
{
	if(connection != nullptr)
	{
		auto portColor = ImGui::ColorFromHash(port->getDataTypeHash());
		ax::NodeEditor::Link(linkID, connection->getID(), id, 
			portColor, Stylesheet::getCurrentSheet().linkThickness);

		if(dataRequested)
		{
			ax::NodeEditor::Flow(linkID, portColor);
			dataRequested = false;
		}
	}
}
