#include "rsp/gui/nodes/InputEventPort.h"
#include "rsp/base/port/InputEventPort.h"
#include "rsp/base/port/OutputEventPort.h"
#include "rsp/gui/ImGuiUtilities.hpp"
#include "rsp/gui/Stylesheet.hpp"
#include "rsp/gui/nodes/OutputPort.h"

namespace rsp::gui
{
InputEventPort::InputEventPort(rsp::InputEventPort* port) : InputPort(port), port(port)
{
	port->registerObserverFlag(EventPort::ObserverFlags::onTriggered, &triggered);
}

void InputEventPort::draw()
{
	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Input);

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
		ax::NodeEditor::Link(linkID, connection->getID(), id, Stylesheet::getCurrentSheet().eventColor,
			Stylesheet::getCurrentSheet().linkThickness);

		if(triggered)
		{
			ax::NodeEditor::Flow(linkID, Stylesheet::getCurrentSheet().eventColor);
			triggered = false;
		}
	}
}

} // namespace rsp::gui