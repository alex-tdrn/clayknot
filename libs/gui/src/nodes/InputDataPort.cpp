#include "rsp/gui/nodes/InputDataPort.h"
#include "rsp/base/port/InputDataPort.hpp"
#include "rsp/gui/ImGuiUtilities.hpp"
#include "rsp/gui/Stylesheet.hpp"
#include "rsp/gui/nodes/OutputPort.h"
#include "rsp/gui/widgets/Viewer.hpp"

namespace rsp::gui
{
InputDataPort::InputDataPort(rsp::DataPort* port) : InputPort(port), DataPort(port)
{
	port->registerObserverFlag(rsp::DataPort::ObserverFlags::onFailedUpdate, &portUpdateFailed);
	port->registerObserverFlag(rsp::DataPort::ObserverFlags::onDataRequested, &dataRequested);

	port->registerObserverFlag(Port::ObserverFlags::onConnected, &connectionStateChanged);
	port->registerObserverFlag(Port::ObserverFlags::onDisconnected, &connectionStateChanged);
}

void InputDataPort::generateViewer()
{
	widget.reset();
	SupportedViewerTypes::find_and_apply([&](auto* t) {
		using ResourceType = std::remove_reference_t<decltype(*t)>;
		auto concretePort = dynamic_cast<rsp::InputDataPort<ResourceType> const*>(port);
		if(!concretePort)
			return false;
		widget = makeViewer(concretePort->getData(), port->getName());
		widget->setMaximumWidth(150);
		return true;
	});
}

void InputDataPort::drawContents()
{
	if(connectionStateChanged)
	{
		generateViewer();
		connectionStateChanged = false;
	}

	ax::NodeEditor::BeginPin(id, ax::NodeEditor::PinKind::Input);

	placeAnchor(getWidgetSize().y);

	if(portUpdateFailed)
	{
		anchorOffset.play();
		portUpdateFailed = false;
	}

	auto anchorPosition = calculateAnchorPosition();
	auto anchorColor = ImGui::ColorFromHash(dataPort->getDataTypeHash());

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

void InputDataPort::drawLink()
{
	if(connection != nullptr)
	{
		auto portColor = ImGui::ColorFromHash(dataPort->getDataTypeHash());
		ax::NodeEditor::Link(linkID, connection->getID(), id, portColor, Stylesheet::getCurrentSheet().linkThickness);

		if(dataRequested)
		{
			ax::NodeEditor::Flow(linkID, portColor);
			dataRequested = false;
		}
	}
}
} // namespace rsp::gui