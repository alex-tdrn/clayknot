#pragma once

#include "rsp/base/Graph.hpp"
#include "rsp/gui/widgets/Viewer.hpp"
#include "rsp/util/ColorRGBA.hpp"

#include <imnodes.h>
#include <unordered_set>

namespace rsp::gui
{
class GraphViewer final : public ViewerOf<Graph>
{
public:
	GraphViewer() = delete;
	GraphViewer(Graph const* data, std::string const& dataName);
	GraphViewer(GraphViewer const&) = delete;
	GraphViewer(GraphViewer&&) = delete;
	auto operator=(GraphViewer const&) -> GraphViewer& = delete;
	auto operator=(GraphViewer&&) -> GraphViewer& = delete;
	~GraphViewer() final;

	auto clone() const -> std::unique_ptr<Widget> final;
	void drawContents() const final;

private:
	struct NodeDrawState
	{
		int id = -1;
		Node const* node = nullptr;
		bool firstDraw = true;
		bool highlighted = false;
		float titleWidth = 0;
		float contentsWidth = 0;
	};

	struct PortDrawState
	{
		int id = -1;
		Port const* port = nullptr;
		std::uint32_t color = ColorRGBA{1.0f}.packed();
		std::unique_ptr<Viewer> viewer = nullptr;
	};

	imnodes::EditorContext* context;
	float maxPortContentsWidth = 200.0f;
	mutable std::unordered_map<Node const*, NodeDrawState> nodeDrawStates;
	mutable std::unordered_map<int, Node const*> idToNode;
	mutable std::unordered_map<Port const*, PortDrawState> portDrawStates;
	mutable int nextAvailableNodeID = 0;
	mutable int nextAvailablePortID = 0;
	mutable std::vector<std::pair<InputPort const*, OutputPort const*>> connections;
	mutable Node const* hoveredNode = nullptr;
	mutable std::unordered_set<Node const*> selectedNodes;

	void drawNode(NodeDrawState& drawState) const;
	static void drawNodeTitleBar(NodeDrawState& drawState);
	static void drawPort(PortDrawState& drawState);
	auto getNodeDrawState(Node const* node) const -> NodeDrawState&;
	auto getPortDrawState(Port const* port) const -> PortDrawState&;
};

} // namespace rsp::gui