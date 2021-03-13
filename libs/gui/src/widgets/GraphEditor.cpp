#include "rsp/gui/widgets/GraphEditor.h"
#include "rsp/algorithms/DecomposeColor.h"
#include "rsp/algorithms/GrayscaleColorNode.h"
#include "rsp/algorithms/MixColors.h"
#include "rsp/algorithms/RandomColorSource.h"
#include "rsp/algorithms/ValueToColor.h"

namespace rsp::gui
{
GraphEditor::GraphEditor(
	Graph* data, std::string const& dataName, std::optional<std::function<void()>> modifiedCallback)
	: EditorOf<Graph>(data, dataName, std::move(modifiedCallback))
{
	disableTitle();
	context = imnodes::EditorContextCreate();
	imnodes::EditorContextSet(context);

	imnodes::EditorContextSet(nullptr);
}

GraphEditor::~GraphEditor()
{
	imnodes::EditorContextFree(context);
}

auto GraphEditor::clone() const -> std::unique_ptr<Widget>
{
	return std::make_unique<GraphEditor>(getData(), getDataName(), getModifiedCallback());
}

void GraphEditor::drawContents() const
{
	imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkCreationOnSnap);
	if(connectingPort != nullptr)
		imnodes::PushColorStyle(imnodes::ColorStyle_Link, getPortDrawState(connectingPort).color);
	else
		imnodes::PushColorStyle(imnodes::ColorStyle_Link, ColorRGBA{1.0f}.packed());

	connections.clear();
	imnodes::EditorContextSet(context);

	imnodes::BeginNodeEditor();

	for(auto const& node : *getData())
		drawNode(getNodeDrawState(node.get()));

	{
		int linkID = 0;
		for(auto& connection : connections)
		{
			auto color = ColorRGBA(ColorRGB::createRandom(connection.first->getDataTypeHash()), 1.0f).packed();
			imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
			if(connectingPort != nullptr)
			{
				imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, color);
				imnodes::PushColorStyle(imnodes::ColorStyle_LinkSelected, color);
			}

			imnodes::Link(linkID++, getPortDrawState(connection.first).id, getPortDrawState(connection.second).id);

			imnodes::PopColorStyle();
			if(connectingPort != nullptr)
			{
				imnodes::PopColorStyle();
				imnodes::PopColorStyle();
			}
		}
	}

	if(droppedConnection.first != nullptr && connectingPort != nullptr)
	{
		auto color = ColorRGBA(ColorRGB(0.0f), 1.0f).packed();
		imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
		imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, color);
		imnodes::PushColorStyle(imnodes::ColorStyle_LinkSelected, color);

		imnodes::Link(-1, getPortDrawState(droppedConnection.first).id, getPortDrawState(droppedConnection.second).id);

		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
	}
	imnodes::EndNodeEditor();

	bool deletThis = false;
	if(ImGui::BeginPopupContextItem("Context menu"))
	{
		if(ImGui::BeginMenu("New node"))
		{
			auto* graph = getData();

			if(ImGui::MenuItem("DecomposeColor"))
				graph->push_back(std::make_unique<rsp::Node>(std::make_unique<rsp::algorithms::DecomposeColor>()));
			if(ImGui::MenuItem("GrayscaleColorNode"))
				graph->push_back(std::make_unique<rsp::Node>(std::make_unique<rsp::algorithms::GrayscaleColorNode>()));
			if(ImGui::MenuItem("MixColors"))
				graph->push_back(std::make_unique<rsp::Node>(std::make_unique<rsp::algorithms::MixColors>()));
			if(ImGui::MenuItem("RandomColorSource"))
				graph->push_back(std::make_unique<rsp::Node>(std::make_unique<rsp::algorithms::RandomColorSource>()));

			ImGui::EndMenu();
		}
		if(imnodes::NumSelectedLinks() > 0 || imnodes::NumSelectedNodes() > 0)
			deletThis = ImGui::MenuItem("Delete");
		ImGui::EndPopup();
	}

	hoveredNode = nullptr;
	if(int hoveredNodeID = -1; imnodes::IsNodeHovered(&hoveredNodeID))
		hoveredNode = idToNode[hoveredNodeID];

	if(int connectingPortID = -1; imnodes::IsLinkStarted(&connectingPortID))
		connectingPort = idToPort[connectingPortID];

	if(int linkID = -1; imnodes::IsLinkDestroyed(&linkID) && linkID >= 0)
	{
		auto* input = connections[linkID].first;
		auto* output = connections[linkID].second;
		input->disconnectFrom(*output);

		if(input == droppedConnection.first)
		{
			droppedConnection.first->connectTo(*droppedConnection.second);

			droppedConnection.first = nullptr;
			droppedConnection.second = nullptr;
		}
	}

	{
		int outputPortID = -1;
		int inputPortID = -1;
		bool createdFromSnap = false;
		if(imnodes::IsLinkCreated(&outputPortID, &inputPortID, &createdFromSnap))
		{
			auto* inputPort = dynamic_cast<rsp::InputPort*>(idToPort[inputPortID]);
			auto* outputPort = dynamic_cast<rsp::OutputPort*>(idToPort[outputPortID]);
			if(inputPort != nullptr && outputPort != nullptr)
			{
				if(inputPort->isConnected())
				{
					droppedConnection.first = inputPort;
					droppedConnection.second = inputPort->getConnectedOutputPort();
				}

				inputPort->connectTo(*outputPort);
			}
		}
	}

	if(ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		connectingPort = nullptr;
		droppedConnection.first = nullptr;
		droppedConnection.second = nullptr;
	}

	if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && imnodes::NumSelectedLinks() > 0)
	{
		std::vector<int> selectedLinks(imnodes::NumSelectedLinks());
		imnodes::GetSelectedLinks(selectedLinks.data());
		for(auto linkID : selectedLinks)
			connections[linkID].first->disconnectFrom(*connections[linkID].second);
		imnodes::ClearLinkSelection();
	}

	if(imnodes::NumSelectedNodes() != static_cast<int>(selectedNodes.size()) || selectedNodes.size() == 1)
	{
		for(auto* node : selectedNodes)
			getNodeDrawState(node).highlighted = false;

		selectedNodes.clear();
		if(imnodes::NumSelectedNodes() > 0)
		{
			std::vector<int> selectedNodeIDs(imnodes::NumSelectedNodes());
			imnodes::GetSelectedNodes(selectedNodeIDs.data());
			for(auto nodeID : selectedNodeIDs)
				selectedNodes.insert(idToNode[nodeID]);
		}

		for(auto* node : selectedNodes)
			getNodeDrawState(node).highlighted = true;
	}

	if(deletThis || (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) &&
						ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
	{
		if(imnodes::NumSelectedLinks() > 0)
		{
			std::vector<int> selectedLinks(imnodes::NumSelectedLinks());
			imnodes::GetSelectedLinks(selectedLinks.data());
			for(auto linkID : selectedLinks)
				connections[linkID].first->disconnectFrom(*connections[linkID].second);
			imnodes::ClearLinkSelection();
		}

		auto* graph = getData();

		for(auto* selectedNode : selectedNodes)
		{
			graph->erase(std::remove_if(graph->begin(), graph->end(),
							 [&](auto const& node) {
								 return node.get() == selectedNode;
							 }),
				graph->end());
		}

		selectedNodes.clear();
		imnodes::ClearNodeSelection();
	}

	imnodes::EditorContextSet(nullptr);
	imnodes::PopAttributeFlag();
	imnodes::PopColorStyle();
}

void GraphEditor::drawNode(NodeDrawState& drawState) const
{
	if(drawState.highlighted || drawState.node == hoveredNode)
		imnodes::PushColorStyle(imnodes::ColorStyle_NodeOutline, ColorRGBA{1.0f}.packed());

	imnodes::BeginNode(drawState.id);

	drawNodeTitleBar(drawState);

	ImGui::BeginGroup();
	for(auto& port : drawState.node->getInputPorts())
		drawPort(getPortDrawState(port));
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	for(auto& port : drawState.node->getOutputPorts())
	{
		drawPort(getPortDrawState(port));
		for(auto* connectedInput : port->getConnectedInputPorts())
			if(portDrawStates.find(connectedInput) != portDrawStates.end())
				connections.emplace_back(std::make_pair(connectedInput, port));
	}
	ImGui::EndGroup();

	imnodes::EndNode();
	drawState.contentsWidth = ImGui::GetItemRectSize().x;

	if(drawState.highlighted || drawState.node == hoveredNode)
		imnodes::PopColorStyle();

	drawState.firstDraw = false;
}

void GraphEditor::drawNodeTitleBar(NodeDrawState& drawState)
{
	imnodes::BeginNodeTitleBar();

	ImGui::BeginGroup();

	if(!drawState.firstDraw && drawState.titleWidth < drawState.contentsWidth)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (drawState.contentsWidth - drawState.titleWidth) / 2);

	if(!drawState.node->getInputPorts().empty())
	{
		if(ImGui::SmallButton("Pull"))
			drawState.node->pull();
		ImGui::SameLine();
	}

	ImGui::Text("%s", drawState.node->getName().data());

	if(!drawState.node->getOutputPorts().empty())
	{
		ImGui::SameLine();
		if(ImGui::SmallButton("Push"))
			drawState.node->push();
	}

	ImGui::EndGroup();

	if(drawState.firstDraw)
		drawState.titleWidth = ImGui::GetItemRectSize().x;

	imnodes::EndNodeTitleBar();
}

void GraphEditor::drawPort(PortDrawState& drawState) const
{
	bool disabled =
		connectingPort != nullptr && drawState.port != connectingPort && !drawState.port->canConnectTo(*connectingPort);

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, drawState.color);

	if(auto* inputPort = dynamic_cast<InputPort*>(drawState.port); inputPort != nullptr)
	{
		float const beginY = ImGui::GetCursorPosY();

		if(disabled)
			imnodes::BeginStaticAttribute(drawState.id);
		else if(drawState.port->isConnected() || drawState.port == connectingPort)
			imnodes::BeginInputAttribute(drawState.id, imnodes::PinShape_QuadFilled);
		else
			imnodes::BeginInputAttribute(drawState.id, imnodes::PinShape_Quad);

		auto* defaultEditor = getPortDrawState(&inputPort->getDefaultPort()).widget.get();
		auto* viewer = static_cast<Viewer*>(drawState.widget.get());

		if(!inputPort->isConnected())
		{
			defaultEditor->draw();
		}
		else
		{
			viewer->updateDataPointer(drawState.port->getDataPointer());
			viewer->draw();
		}

		if(connectingPort != nullptr && connectingPort != drawState.port)
		{
			float currentHeight = ImGui::GetCursorPosY() - beginY;
			float maxHeight = std::max(viewer->getLastSize().y, defaultEditor->getLastSize().y);
			if(currentHeight < maxHeight)
				ImGui::Dummy(ImVec2(10, maxHeight - currentHeight));
		}

		if(disabled)
			imnodes::EndStaticAttribute();
		else
			imnodes::EndInputAttribute();
	}
	else
	{
		if(disabled)
			imnodes::BeginStaticAttribute(drawState.id);
		else if(drawState.port->isConnected() || drawState.port == connectingPort)
			imnodes::BeginOutputAttribute(drawState.id, imnodes::PinShape_TriangleFilled);
		else
			imnodes::BeginOutputAttribute(drawState.id, imnodes::PinShape_Triangle);

		drawState.widget->draw();

		if(disabled)
			imnodes::EndStaticAttribute();
		else
			imnodes::EndOutputAttribute();
	}
	imnodes::PopColorStyle();
}

auto GraphEditor::getNodeDrawState(Node* node) const -> NodeDrawState&
{
	if(nodeDrawStates.find(node) == nodeDrawStates.end())
	{
		auto& drawState = nodeDrawStates[node];
		drawState.node = node;
		drawState.id = nextAvailableNodeID++;
		idToNode[drawState.id] = node;
	}
	return nodeDrawStates[node];
}

auto GraphEditor::getPortDrawState(Port* port) const -> PortDrawState&
{
	if(portDrawStates.find(port) == portDrawStates.end())
	{
		auto& drawState = portDrawStates[port];
		drawState.port = port;
		drawState.id = nextAvailablePortID++;
		drawState.widget = Viewer::create(port->getDataTypeHash(), port->getDataPointer(), port->getName());
		drawState.widget->setMaximumWidth(maxPortContentsWidth);
		drawState.color = ColorRGBA(ColorRGB::createRandom(port->getDataTypeHash()), 1.0f).packed();
		idToPort[drawState.id] = port;

		if(auto* inputPort = dynamic_cast<InputPort*>(port); inputPort != nullptr)
		{
			auto* defaultPort = &inputPort->getDefaultPort();
			auto& defaultDrawState = portDrawStates[defaultPort];
			defaultDrawState.port = defaultPort;
			defaultDrawState.id = nextAvailablePortID++;
			defaultDrawState.widget = Editor::create(
				defaultPort->getDataTypeHash(), defaultPort->getDataPointer(), inputPort->getName(), [=]() {
					defaultPort->updateTimestamp();
					defaultPort->push();
				});
			defaultDrawState.widget->setMaximumWidth(maxPortContentsWidth);
			defaultDrawState.color = ColorRGBA(ColorRGB::createRandom(port->getDataTypeHash()), 1.0f).packed();
			idToPort[defaultDrawState.id] = defaultPort;
		}
	}
	return portDrawStates[port];
}

} // namespace rsp::gui