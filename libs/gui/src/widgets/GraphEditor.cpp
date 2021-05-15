#include "rsp/gui/widgets/GraphEditor.h"

#include "NodeEditors.hpp"
#include "PortEditors.hpp"
#include "SelectionManager.hpp"
#include "WidgetCache.hpp"
#include "rsp/algorithms/DecomposeColor.h"
#include "rsp/algorithms/GrayscaleColorNode.h"
#include "rsp/algorithms/MixColors.h"
#include "rsp/algorithms/RandomColorSource.h"
#include "rsp/algorithms/ValueToColor.h"
#include "rsp/base/AlgorithmNode.hpp"
#include "rsp/base/ConstantNode.hpp"
#include "rsp/base/Port.hpp"

namespace rsp::gui
{
GraphEditor::GraphEditor(
	Graph* data, std::string const& dataName, std::optional<std::function<void()>> modifiedCallback)
	: EditorOf<Graph>(data, dataName, std::move(modifiedCallback))
{
	portCache = std::make_unique<impl::WidgetCache<Port, impl::PortEditor>>(&impl::createPortEditor);

	nodeCache = std::make_unique<impl::WidgetCache<Node, impl::NodeEditor>>([&](Node* node, int id) {
		return impl::createNodeEditor(node, id, portCache.get(), modificationCallback);
	});

	selectionManager = std::make_unique<impl::SelectionManager<false>>(nodeCache.get(), portCache.get());

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
	imnodes::EditorContextSet(context);

	drawGraph();
	drawMenus();
	updateConnections();
	selectionManager->update();
	handleMouseInteractions();

	if(modificationCallback.has_value())
	{
		if((*modificationCallback)())
			modificationCallback = std::nullopt;
	}

	imnodes::EditorContextSet(nullptr);
}

void GraphEditor::drawGraph() const
{
	imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkCreationOnSnap);
	if(newConnectionInProgress)
		imnodes::PushColorStyle(
			imnodes::ColorStyle_Link, portCache->getWidget(&newConnectionInProgress->startingPort).getColor());
	else
		imnodes::PushColorStyle(imnodes::ColorStyle_Link, ColorRGBA{1.0f}.packed());

	connections.clear();

	imnodes::BeginNodeEditor();

	for(auto const& node : *getData())
	{
		nodeCache->getWidget(node.get()).draw();
		for(auto* output : node->getOutputPorts())
			for(auto* input : output->getConnectedInputPorts())
				if(portCache->hasWidget(input))
					connections.emplace_back(std::make_pair(input, output));
	}

	{
		int linkID = 0;

		for(auto& connection : connections)
		{
			if(newConnectionInProgress && newConnectionInProgress->endingPort != nullptr)
			{
				auto snapConnection =
					std::pair(&newConnectionInProgress->startingPort, newConnectionInProgress->endingPort);

				if((snapConnection.first == connection.first && snapConnection.second == connection.second) ||
					(snapConnection.first == connection.second && snapConnection.second == connection.first))
				{
					linkID++;
					continue;
				}
			}
			auto color = ColorRGBA(ColorRGB::createRandom(connection.first->getDataTypeHash()), 1.0f).packed();
			imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
			if(newConnectionInProgress)
			{
				imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, color);
				imnodes::PushColorStyle(imnodes::ColorStyle_LinkSelected, color);
			}
			imnodes::Link(linkID++, portCache->getWidget(connection.first).getID(),
				portCache->getWidget(connection.second).getID());

			imnodes::PopColorStyle();
			if(newConnectionInProgress)
			{
				imnodes::PopColorStyle();
				imnodes::PopColorStyle();
			}
		}
	}

	if(newConnectionInProgress && newConnectionInProgress->droppedConnection)
	{
		auto color = ColorRGBA(ColorRGB(0.0f), 1.0f).packed();
		imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
		imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, color);
		imnodes::PushColorStyle(imnodes::ColorStyle_LinkSelected, color);

		imnodes::Link(-1, portCache->getWidget(newConnectionInProgress->droppedConnection->first).getID(),
			portCache->getWidget(newConnectionInProgress->droppedConnection->second).getID());

		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
	}
	imnodes::EndNodeEditor();
	imnodes::PopAttributeFlag();
	imnodes::PopColorStyle();
}

void GraphEditor::drawMenus() const
{
	bool deletThis = false;
	if(ImGui::BeginPopupContextItem("Context menu"))
	{
		if(ImGui::BeginMenu("New node"))
		{
			auto* graph = getData();
			if(ImGui::BeginMenu("Algorithm"))
			{
				if(ImGui::MenuItem("DecomposeColor"))
					graph->push_back(
						std::make_unique<rsp::AlgorithmNode>(std::make_unique<rsp::algorithms::DecomposeColor>()));
				if(ImGui::MenuItem("GrayscaleColorNode"))
					graph->push_back(
						std::make_unique<rsp::AlgorithmNode>(std::make_unique<rsp::algorithms::GrayscaleColorNode>()));
				if(ImGui::MenuItem("MixColors"))
					graph->push_back(
						std::make_unique<rsp::AlgorithmNode>(std::make_unique<rsp::algorithms::MixColors>()));
				if(ImGui::MenuItem("RandomColorSource"))
					graph->push_back(
						std::make_unique<rsp::AlgorithmNode>(std::make_unique<rsp::algorithms::RandomColorSource>()));
				ImGui::EndMenu();
			}

			if(ImGui::MenuItem("Constant"))
				graph->push_back(std::make_unique<rsp::ConstantNode>());
			ImGui::EndMenu();
		}

		if(imnodes::NumSelectedLinks() > 0 || imnodes::NumSelectedNodes() > 0)
		{
			ImGui::Separator();
			if(!selectionManager->getSelectedNodes().empty())
			{
				const auto& nodes = selectionManager->getSelectedNodes();

				bool anyInputs = std::any_of(nodes.begin(), nodes.end(), [](auto node) {
					return !node->getInputPorts().empty();
				});
				if(anyInputs && ImGui::MenuItem("Copy inputs to new constant node"))
				{
					auto constantNode = std::make_unique<rsp::ConstantNode>();
					for(auto* node : selectionManager->getSelectedNodes())
					{
						for(auto* inputPort : node->getInputPorts())
							constantNode->addPort(std::unique_ptr<rsp::OutputPort>(
								dynamic_cast<OutputPort*>(inputPort->createCompatiblePort().release())));
					}

					getData()->push_back(std::move(constantNode));
				}
			}
			deletThis = ImGui::MenuItem("Delete");
		}
		ImGui::EndPopup();
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

		for(auto* selectedNode : selectionManager->getSelectedNodes())
		{
			graph->erase(std::remove_if(graph->begin(), graph->end(),
							 [&](auto const& node) {
								 return node.get() == selectedNode;
							 }),
				graph->end());
		}

		imnodes::ClearNodeSelection();
	}
}

void GraphEditor::updateConnections() const
{
	if(int connectingPortID = -1; imnodes::IsLinkStarted(&connectingPortID))
	{
		newConnectionInProgress.emplace(ConnectionChange{*portCache->getWidget(connectingPortID).getPort()});

		for(const auto& it : portCache->getMap())
		{
			const auto& portEditor = it.second;

			portEditor->setEnabled(false);
			portEditor->setStableHeight(true);
			if(portEditor->getPort()->canConnectTo(newConnectionInProgress->startingPort))
			{
				portEditor->setEnabled(true);
			}
		}
	}

	if(int outputPortID = -1, inputPortID = -1; imnodes::IsLinkCreated(&outputPortID, &inputPortID))
	{
		auto* inputPort = dynamic_cast<rsp::InputPort*>(portCache->getWidget(inputPortID).getPort());
		auto* outputPort = dynamic_cast<rsp::OutputPort*>(portCache->getWidget(outputPortID).getPort());

		if(newConnectionInProgress->endingPort != nullptr)
			newConnectionInProgress->startingPort.disconnectFrom(*newConnectionInProgress->endingPort);

		if(inputPort == &newConnectionInProgress->startingPort)
			newConnectionInProgress->endingPort = outputPort;
		else
			newConnectionInProgress->endingPort = inputPort;

		restoreDroppedConnection();

		if(inputPort->isConnected())
			newConnectionInProgress->droppedConnection = std::pair(inputPort, inputPort->getConnectedOutputPort());

		inputPort->connectTo(*outputPort);
	}

	if(int dummy = -1;
		newConnectionInProgress && newConnectionInProgress->endingPort != nullptr && !imnodes::IsPinHovered(&dummy))
	{
		newConnectionInProgress->startingPort.disconnectFrom(*newConnectionInProgress->endingPort);
		newConnectionInProgress->endingPort = nullptr;
		restoreDroppedConnection();
	}
}

void GraphEditor::handleMouseInteractions() const
{
	if(ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		newConnectionInProgress = std::nullopt;

		for(const auto& it : portCache->getMap())
		{
			const auto& portEditor = it.second;

			portEditor->setEnabled(true);
			portEditor->setStableHeight(false);
		}
	}

	if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && imnodes::NumSelectedLinks() > 0)
	{
		std::vector<int> selectedLinks(imnodes::NumSelectedLinks());
		imnodes::GetSelectedLinks(selectedLinks.data());
		for(auto linkID : selectedLinks)
			connections[linkID].first->disconnectFrom(*connections[linkID].second);
		imnodes::ClearLinkSelection();
	}
}

void GraphEditor::restoreDroppedConnection() const
{
	if(newConnectionInProgress && newConnectionInProgress->droppedConnection)
	{
		newConnectionInProgress->droppedConnection->first->connectTo(
			*newConnectionInProgress->droppedConnection->second);
		newConnectionInProgress->droppedConnection = std::nullopt;
	}
}

} // namespace rsp::gui