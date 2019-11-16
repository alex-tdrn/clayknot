#include "OutputDataPin.h"
#include "rshp/base/OutputDataPort.hpp"
#include "OutputDataPinDelegate.h"
#include "ImGuiUtilities.hpp"

#include <glm/glm.hpp>

OutputDataPin::OutputDataPin(AbstractDataPort* port)
	: OutputPin(port), port(port)
{
	delegate = OutputDataPinDelegate::create(port);
}

void OutputDataPin::draw()
{

	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Output);
	ImGui::BeginGroup();
	ImGui::Text(port->getName().c_str());

	delegate->draw();
	
	ImGui::EndGroup();
	
	auto anchorPosition = calculateAnchorPosition();
	auto anchorColor = ImGui::ColorFromHash(port->getDataTypeHash());

	if(port->isConnected())
		ImGui::DrawCircle(anchorPosition, 5, anchorColor);
	else
		ImGui::DrawCircle(anchorPosition, 5, {0, 0, 0, 1}, anchorColor);

	ax::NodeEditor::PinPivotRect(anchorPosition, anchorPosition);

	ax::NodeEditor::EndPin();
}

ImVec2 OutputDataPin::calculateSize() const
{
	return ImGui::CalcTextSize(port->getName().c_str());
}
