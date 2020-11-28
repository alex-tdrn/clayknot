#include "rsp/gui/nodes/OutputPort.h"
#include "rsp/gui/Animation.hpp"
#include "rsp/gui/ImGuiUtilities.hpp"
#include "rsp/gui/Stylesheet.hpp"
#include "rsp/gui/nodes/InputPort.h"
#include "rsp/util/Bounded.hpp"

#include <assert.h>
#include <imgui_node_editor.h>
#include <unordered_set>

namespace rsp::gui
{
OutputPort::OutputPort(rsp::Port* port) : AbstractPort(port)
{
	for(auto connection : port->getConnections())
		if(auto pin = getPinForPort(connection); pin != nullptr)
			connect(pin);
}

ImVec2 OutputPort::calculateAnchorPosition() const
{
	auto& currentStyle = Stylesheet::getCurrentSheet();
	auto min = ImGui::GetItemRectMin();
	auto max = ImGui::GetItemRectMax();
	auto w = max.x - min.x;
	auto x = min.x + w / 2 +
			 anchorOffset.get(currentStyle.animatedAnchorOffset, currentStyle.anchorOffset,
				 currentStyle.animatedAnchorOffsetDuration, AnimationCurve::spring);

	auto h = max.y - min.y;
	auto y = min.y + h / 2;
	return {x, y};
}

void OutputPort::drawContents()
{
	drawWidget();
	ImGui::SameLine();

	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Output);

	ImGui::SameLine();
	auto anchorColor = ImGui::ColorFromHash(port->getDataTypeHash());

	placeAnchor(ImGui::GetItemRectSize().y);

	auto anchorPosition = calculateAnchorPosition();
	if(port->isConnected())
		ImGui::DrawCircle(anchorPosition, 5, anchorColor);
	else
		ImGui::DrawCircle(anchorPosition, 5, {0, 0, 0, 1}, anchorColor);

	ax::NodeEditor::PinPivotRect(anchorPosition, anchorPosition);

	ax::NodeEditor::EndPin();
}

bool OutputPort::canConnect(AbstractPort const* inputPort) const
{
	if(!dynamic_cast<InputPort const*>(inputPort))
		return false;
	return inputPort->canConnect(this);
}

void OutputPort::connect(AbstractPort* inputPort)
{
	assert(dynamic_cast<InputPort*>(inputPort));

	dynamic_cast<InputPort*>(inputPort)->connect(this);
}

} // namespace rsp::gui

// TODO ??????????/
// OutputDataPort::OutputDataPort(rsp::DataPort* port, bool fixedNode) : OutputPort(port), DataPort(port)
// {
// 	if(fixedNode)
// 	{
// 		SupportedEditorTypes::find_and_apply([&](auto* t) {
// 			using ResourceType = std::remove_reference_t<decltype(*t)>;
// 			auto concretePort = dynamic_cast<rsp::OutputDataPort<ResourceType> const*>(port);
// 			if(!concretePort)
// 				return false;
// 			auto fixedParent = dynamic_cast<rsp::FixedSource<ResourceType>*>(concretePort->getParent());
// 			widget = makeOwningEditor(fixedParent->getFixedData(), port->getName(),
// 				[fixedParent = fixedParent](auto const& modifiedData) { fixedParent->setFixedData(modifiedData); });
// 			widget->setMaximumWidth(150);
// 			return true;
// 		});
// 	}
// 	else
// 	{
// 		SupportedViewerTypes::find_and_apply([&](auto* t) {
// 			using ResourceType = std::remove_reference_t<decltype(*t)>;
// 			auto concretePort = dynamic_cast<rsp::OutputDataPort<ResourceType> const*>(port);
// 			if(!concretePort)
// 				return false;
// 			widget = makeViewer(concretePort->getData(), port->getName());
// 			widget->setMaximumWidth(150);
// 			return true;
// 		});
// 	}
// 	assert(widget != nullptr);
// }