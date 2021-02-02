#include "rsp/gui/nodes/InputPort.h"
#include "rsp/gui/Animation.hpp"
#include "rsp/gui/ImGuiUtilities.hpp"
#include "rsp/gui/Stylesheet.hpp"
#include "rsp/gui/UniqueID.hpp"
#include "rsp/gui/nodes/OutputPort.h"
#include "rsp/gui/widgets/Viewer.hpp"
#include "rsp/gui/widgets/Widget.hpp"
#include "rsp/util/Bounded.hpp"

#include <cassert>
#include <glm/glm.hpp>
#include <memory>
#include <type_traits>
#include <unordered_set>

namespace rsp::gui
{
InputPort::InputPort(rsp::Port* port) : AbstractPort(port), linkID(uniqueID())
{
	auto connections = port->getConnectedPorts();
	assert(connections.size() <= 1);
	for(const auto* connection : connections)
		if(auto* pin = getPinForPort(connection); pin != nullptr)
			connect(pin);
}

auto InputPort::calculateAnchorPosition() const -> ImVec2
{
	auto currentStyle = Stylesheet::getCurrentSheet();
	auto min = ImGui::GetItemRectMin();
	auto max = ImGui::GetItemRectMax();
	auto w = max.x - min.x;
	auto x = min.x + w / 2 -
			 anchorOffset.get(currentStyle.animatedAnchorOffset.getVal(), currentStyle.anchorOffset.getVal(),
				 currentStyle.animatedAnchorOffsetDuration, AnimationCurve::spring);

	auto h = max.y - min.y;
	auto y = min.y + h / 2;
	return {x, y};
}

void InputPort::generateViewer()
{
	widget.reset();
	SupportedViewerTypes::find_and_apply([&](auto* t) {
		using ResourceType = std::remove_reference_t<decltype(*t)>;
		auto concretePort = dynamic_cast<rsp::InputPortOf<ResourceType> const*>(port);
		if(!concretePort)
			return false;
		widget = makeViewer(*concretePort, port->getName());
		widget->setMaximumWidth(150);
		return true;
	});
}

void InputPort::drawContents()
{
	if(connection == nullptr)
		widget.reset();

	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Input);

	placeAnchor(getWidgetSize().y);

	auto anchorPosition = calculateAnchorPosition();
	auto anchorColor = ImGui::ColorFromHash(port->getDataTypeHash());

	if(port->isConnected())
		ImGui::DrawCircle(anchorPosition, 5, anchorColor);
	else
		ImGui::DrawCircle(anchorPosition, 5, {0, 0, 0, 1}, anchorColor);

	ImGui::SameLine();
	ax::NodeEditor::PinPivotRect(anchorPosition, anchorPosition);
	ax::NodeEditor::EndPin();

	ImGui::SameLine();
	drawWidget();
}

void InputPort::drawLink()
{
	if(connection != nullptr)
	{
		auto portColor = ImGui::ColorFromHash(port->getDataTypeHash());
		ax::NodeEditor::Link(
			linkID, connection->getID(), id, portColor, Stylesheet::getCurrentSheet().linkThickness.getVal());
	}
}

auto InputPort::canConnect(const AbstractPort* outputPort) const -> bool
{
	if(dynamic_cast<OutputPort const*>(outputPort) == nullptr)
		return false;
	return port->canConnectTo(*outputPort->getPort());
}

void InputPort::connect(AbstractPort* outputPort)
{
	assert(dynamic_cast<OutputPort*>(outputPort));
	assert(canConnect(outputPort));

	port->connectTo(*outputPort->getPort());
	connection = dynamic_cast<OutputPort*>(outputPort);

	generateViewer();
}

} // namespace rsp::gui