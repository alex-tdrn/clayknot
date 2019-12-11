#include "rsp/gui/nodes/OutputEventPort.h"
#include "rsp/base/port/OutputEventPort.h"
#include "rsp/gui/ImGuiUtilities.hpp"
#include "rsp/gui/Stylesheet.hpp"

#include <glm/glm.hpp>

namespace rsp::gui
{
OutputEventPort::OutputEventPort(rsp::OutputEventPort* port) : OutputPort(port), port(port)
{
}

void OutputEventPort::draw()
{
	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Output);

	if(port->getTimesTriggered() > triggerCount)
	{
		triggerCount = port->getTimesTriggered();
		if(!port->isConnected())
			anchorOffset.play();
	}

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

ImVec2 OutputEventPort::calculateSize() const
{
	return ImGui::CalcTextSize(port->getName().c_str());
}

} // namespace rsp::gui