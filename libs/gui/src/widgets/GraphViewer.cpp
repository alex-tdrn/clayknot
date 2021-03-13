#include "rsp/gui/widgets/GraphViewer.h"

namespace rsp::gui
{
GraphViewer::GraphViewer(Graph const* data, std::string const& dataName) : ViewerOf<Graph>(data, dataName)
{
	disableTitle();
	context = imnodes::EditorContextCreate();
	imnodes::EditorContextSet(context);

	imnodes::EditorContextSet(nullptr);
}

GraphViewer::~GraphViewer()
{
	imnodes::EditorContextFree(context);
}

auto GraphViewer::clone() const -> std::unique_ptr<Widget>
{
	return std::make_unique<GraphViewer>(getData(), getDataName());
}

void GraphViewer::drawContents() const
{
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
			imnodes::Link(linkID++, getPortDrawState(connection.first).id, getPortDrawState(connection.second).id);
			imnodes::PopColorStyle();
		}
	}

	imnodes::EndNodeEditor();

	hoveredNode = nullptr;
	if(int hoveredNodeID = -1; imnodes::IsNodeHovered(&hoveredNodeID))
		hoveredNode = idToNode[hoveredNodeID];

	if(imnodes::NumSelectedNodes() != static_cast<int>(selectedNodes.size()) || selectedNodes.size() == 1)
	{
		for(const auto* node : selectedNodes)
			getNodeDrawState(node).highlighted = false;

		selectedNodes.clear();
		if(imnodes::NumSelectedNodes() > 0)
		{
			std::vector<int> selectedNodeIDs(imnodes::NumSelectedNodes());
			imnodes::GetSelectedNodes(selectedNodeIDs.data());

			for(auto nodeID : selectedNodeIDs)
			{
				selectedNodes.insert(idToNode[nodeID]);
			}
		}

		for(const auto* node : selectedNodes)
			getNodeDrawState(node).highlighted = true;
	}

	imnodes::EditorContextSet(nullptr);
}

void GraphViewer::drawNode(NodeDrawState& drawState) const
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

void GraphViewer::drawNodeTitleBar(NodeDrawState& drawState)
{
	imnodes::BeginNodeTitleBar();

	ImGui::BeginGroup();

	if(!drawState.firstDraw && drawState.titleWidth < drawState.contentsWidth)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (drawState.contentsWidth - drawState.titleWidth) / 2);

	ImGui::Text("%s", drawState.node->getName().data());

	ImGui::EndGroup();

	if(drawState.firstDraw)
		drawState.titleWidth = ImGui::GetItemRectSize().x;

	imnodes::EndNodeTitleBar();
}

void GraphViewer::drawPort(PortDrawState& drawState)
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, drawState.color);
	imnodes::PushColorStyle(imnodes::ColorStyle_PinHovered, drawState.color);

	if(dynamic_cast<InputPort const*>(drawState.port) != nullptr)
	{
		if(drawState.port->isConnected())
			imnodes::BeginInputAttribute(drawState.id, imnodes::PinShape_QuadFilled);
		else
			imnodes::BeginInputAttribute(drawState.id, imnodes::PinShape_Quad);

		drawState.viewer->updateDataPointer(drawState.port->getDataPointer());
		drawState.viewer->draw();

		imnodes::EndInputAttribute();
	}
	else
	{
		if(drawState.port->isConnected())
			imnodes::BeginOutputAttribute(drawState.id, imnodes::PinShape_TriangleFilled);
		else
			imnodes::BeginOutputAttribute(drawState.id, imnodes::PinShape_Triangle);

		drawState.viewer->draw();

		imnodes::EndOutputAttribute();
	}
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
}

auto GraphViewer::getNodeDrawState(Node const* node) const -> NodeDrawState&
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

auto GraphViewer::getPortDrawState(Port const* port) const -> PortDrawState&
{
	if(portDrawStates.find(port) == portDrawStates.end())
	{
		auto& drawState = portDrawStates[port];
		drawState.port = port;
		drawState.id = nextAvailablePortID++;
		drawState.viewer = Viewer::create(port->getDataTypeHash(), port->getDataPointer(), port->getName());
		drawState.viewer->setMaximumWidth(maxPortContentsWidth);
		drawState.color = ColorRGBA(ColorRGB::createRandom(port->getDataTypeHash()), 1.0f).packed();
	}
	return portDrawStates[port];
}

} // namespace rsp::gui