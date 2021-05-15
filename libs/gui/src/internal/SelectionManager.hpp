#pragma once

#include "NodeEditors.hpp"
#include "NodeViewers.hpp"
#include "PortEditors.hpp"
#include "PortViewers.hpp"
#include "WidgetCache.hpp"

#include <imnodes.h>
#include <type_traits>
#include <unordered_set>

namespace rsp::gui::impl
{
template <bool Const>
class SelectionManager
{
public:
	using Node = std::conditional_t<Const, Node const, Node>;
	using Port = std::conditional_t<Const, Port const, Port>;
	using NodeWidget = std::conditional_t<Const, NodeViewer, NodeEditor>;
	using PortWidget = std::conditional_t<Const, PortViewer, PortEditor>;

	SelectionManager() = delete;
	SelectionManager(WidgetCache<Node, NodeWidget>* nodeCache, WidgetCache<Port, PortWidget>* portCache);
	SelectionManager(SelectionManager const&) = delete;
	SelectionManager(SelectionManager&&) = delete;
	auto operator=(SelectionManager const&) -> SelectionManager& = delete;
	auto operator=(SelectionManager&&) -> SelectionManager& = delete;
	~SelectionManager() = default;

	void update();
	auto getSelectedNodes() const -> std::unordered_set<Node*> const&;

private:
	WidgetCache<Node, NodeWidget>* nodeCache;
	WidgetCache<Port, PortWidget>* portCache;
	std::unordered_set<Node*> selectedNodes;
	Node* hoveredNode = nullptr; // TODO add hovered pins and connection as well (why not?)
};

template <bool Const>
SelectionManager<Const>::SelectionManager(
	WidgetCache<Node, NodeWidget>* nodeCache, WidgetCache<Port, PortWidget>* portCache)
	: nodeCache(nodeCache), portCache(portCache)
{
}

template <bool Const>
inline void SelectionManager<Const>::update()
{
	if(imnodes::NumSelectedNodes() != static_cast<int>(selectedNodes.size()) || selectedNodes.size() == 1)
	{
		for(auto* node : selectedNodes)
			nodeCache->getWidget(node).setHighlighted(false);

		selectedNodes.clear();
		if(imnodes::NumSelectedNodes() > 0)
		{
			std::vector<int> selectedNodeIDs(imnodes::NumSelectedNodes());
			imnodes::GetSelectedNodes(selectedNodeIDs.data());

			for(auto nodeID : selectedNodeIDs)
			{
				selectedNodes.insert(nodeCache->getWidget(nodeID).getNode());
			}
		}

		for(auto* node : selectedNodes)
			nodeCache->getWidget(node).setHighlighted(true);
	}

	{
		Node* newHoveredNode = nullptr;
		int hoveredNodeID = -1;
		if(imnodes::IsNodeHovered(&hoveredNodeID))
		{
			auto& newHoveredNodeViewer = nodeCache->getWidget(hoveredNodeID);
			newHoveredNodeViewer.setHighlighted(true);
			newHoveredNode = newHoveredNodeViewer.getNode();
		}

		if(hoveredNode != nullptr && newHoveredNode != hoveredNode &&
			selectedNodes.find(hoveredNode) == selectedNodes.end())
			nodeCache->getWidget(hoveredNode).setHighlighted(false);

		hoveredNode = newHoveredNode;
	}
}

template <bool Const>
inline auto SelectionManager<Const>::getSelectedNodes() const -> std::unordered_set<Node*> const&
{
	return selectedNodes;
}

} // namespace rsp::gui::impl
