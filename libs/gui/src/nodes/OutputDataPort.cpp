#include "rsp/gui/nodes/OutputDataPort.h"
#include "rsp/base/node/FixedSource.hpp"
#include "rsp/base/port/OutputDataPort.hpp"
#include "rsp/gui/ImGuiUtilities.hpp"
#include "rsp/gui/widgets/Editor.hpp"
#include "rsp/gui/widgets/Viewer.hpp"

#include <glm/glm.hpp>

namespace rsp::gui
{
OutputDataPort::OutputDataPort(rsp::DataPort* port, bool fixedNode) : OutputPort(port), DataPort(port)
{
	if(fixedNode)
	{
		SupportedEditorTypes::find_and_apply([&](auto* t) {
			using ResourceType = std::remove_reference_t<decltype(*t)>;
			auto concretePort = dynamic_cast<rsp::OutputDataPort<ResourceType> const*>(port);
			if(!concretePort)
				return false;
			auto fixedParent = dynamic_cast<rsp::FixedSource<ResourceType>*>(concretePort->getParent());
			widget = makeOwningEditor(fixedParent->getFixedData(), port->getName(),
				[fixedParent = fixedParent](auto const& modifiedData) { fixedParent->setFixedData(modifiedData); });
			widget->setMaximumWidth(150);
			return true;
		});
	}
	else
	{
		SupportedViewerTypes::find_and_apply([&](auto* t) {
			using ResourceType = std::remove_reference_t<decltype(*t)>;
			auto concretePort = dynamic_cast<rsp::OutputDataPort<ResourceType> const*>(port);
			if(!concretePort)
				return false;
			widget = makeViewer(concretePort->getData(), port->getName());
			widget->setMaximumWidth(150);
			return true;
		});
	}
	assert(widget != nullptr);
}

void OutputDataPort::drawContents()
{
	drawWidget();
	ImGui::SameLine();

	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Output);

	ImGui::SameLine();
	auto anchorColor = ImGui::ColorFromHash(dataPort->getDataTypeHash());

	placeAnchor(ImGui::GetItemRectSize().y);

	auto anchorPosition = calculateAnchorPosition();
	if(port->isConnected())
		ImGui::DrawCircle(anchorPosition, 5, anchorColor);
	else
		ImGui::DrawCircle(anchorPosition, 5, {0, 0, 0, 1}, anchorColor);

	ax::NodeEditor::PinPivotRect(anchorPosition, anchorPosition);

	ax::NodeEditor::EndPin();
}

} // namespace rsp::gui