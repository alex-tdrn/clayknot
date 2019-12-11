#include "rsp/gui/nodes/OutputDataPort.h"
#include "rsp/base/port/OutputDataPort.hpp"
#include "rsp/gui/ImGuiUtilities.hpp"
#include "rsp/gui/nodes/OutputDataPortDelegate.h"

#include <glm/glm.hpp>

namespace rsp::gui
{
OutputDataPort::OutputDataPort(rsp::DataPort* port) : OutputPort(port), port(port)
{
	delegate = OutputDataPortDelegate::create(port, port->getName());
}

void OutputDataPort::draw()
{
	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Output);
	ImGui::BeginGroup();
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

ImVec2 OutputDataPort::calculateSize() const
{
	return ImGui::CalcTextSize(port->getName().c_str());
}

} // namespace rsp::gui