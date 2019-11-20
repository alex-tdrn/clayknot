#include "InputEventPort.h"
#include "OutputPort.h"
#include "Stylesheet.hpp"
#include "ImGuiUtilities.hpp"
#include "rshp/base/port/OutputEventPort.hpp"
#include "rshp/base/port/InputEventPort.hpp"


InputEventPort::InputEventPort(rshp::base::InputEventPort* port)
	: InputPort(port), port(port)
{
}

void InputEventPort::draw()
{
	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Input);

	justTriggered = port->getTimesTriggered() > triggerCount;
	triggerCount = port->getTimesTriggered();
	if(justTriggered && !port->isConnected())
		anchorOffset.play();

	ImGui::PushStyleColor(ImGuiCol_Text, Stylesheet::getCurrentSheet().eventTextColor);
	ImGui::Text(port->getName().c_str());
	ImGui::PopStyleColor();

	auto anchorPosition = calculateAnchorPosition();

	if(port->isConnected())
		ImGui::DrawDiamond(anchorPosition, 5, Stylesheet::getCurrentSheet().eventColor);
	else
		ImGui::DrawDiamond(anchorPosition, 5, {0, 0, 0, 1}, Stylesheet::getCurrentSheet().eventColor);

	ax::NodeEditor::PinPivotRect(anchorPosition, anchorPosition);

	ax::NodeEditor::EndPin();
}

ImVec2 InputEventPort::calculateSize() const
{
	return ImGui::CalcTextSize(port->getName().c_str());
}

void InputEventPort::drawLink()
{
	if(connection != nullptr)
	{
		ax::NodeEditor::Link(linkID, connection->getID(), id,
			Stylesheet::getCurrentSheet().eventColor, Stylesheet::getCurrentSheet().linkThickness);
		
		if(justTriggered)
		{
			ax::NodeEditor::Flow(linkID, Stylesheet::getCurrentSheet().eventColor);
			justTriggered = false;
		}
	}
}
