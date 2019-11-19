#include "OutputEventPin.h"
#include "Stylesheet.hpp"
#include "rshp/base/OutputEventPort.hpp"
#include "ImGuiUtilities.hpp"

#include <glm/glm.hpp>

OutputEventPin::OutputEventPin(rshp::base::OutputEventPort* port)
	: OutputPin(port), port(port)
{
}

void OutputEventPin::draw()
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

ImVec2 OutputEventPin::calculateSize() const
{
	return ImGui::CalcTextSize(port->getName().c_str());
}
