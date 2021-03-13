#pragma once

#include "rsp/base/Graph.hpp"
#include "rsp/gui/widgets/Editor.hpp"
#include "rsp/gui/widgets/Viewer.hpp"
#include "rsp/util/ColorRGBA.hpp"

#include <imnodes.h>

namespace rsp::gui
{
class GraphEditor final : public EditorOf<Graph>
{
public:
	GraphEditor() = delete;
	GraphEditor(
		Graph* data, std::string const& dataName, std::optional<std::function<void()>> modifiedCallback = std::nullopt);
	GraphEditor(GraphEditor const&) = delete;
	GraphEditor(GraphEditor&&) = delete;
	auto operator=(GraphEditor const&) = delete;
	auto operator=(GraphEditor&&) = delete;
	~GraphEditor() final;

	auto clone() const -> std::unique_ptr<Widget> final;
	void drawContents() const final;

private:
	struct NodeDrawState
	{
		int id = -1;
		Node* node = nullptr;
		bool firstDraw = true;
		bool highlighted = false;
		float titleWidth = 0;
		float contentsWidth = 0;
	};

	struct PortDrawState
	{
		int id = -1;
		Port* port = nullptr;
		std::uint32_t color = ColorRGBA{1.0f}.packed();
		std::unique_ptr<Widget> widget = nullptr;
	};

	imnodes::EditorContext* context;
	float maxPortContentsWidth = 200.0f;
	mutable std::unordered_map<Node*, NodeDrawState> nodeDrawStates;
	mutable std::unordered_map<int, Node*> idToNode;
	mutable std::unordered_map<Port*, PortDrawState> portDrawStates;
	mutable std::unordered_map<int, Port*> idToPort;
	mutable int nextAvailableNodeID = 0;
	mutable int nextAvailablePortID = 0;
	mutable std::vector<std::pair<InputPort*, OutputPort*>> connections;
	mutable std::unordered_set<Node*> selectedNodes;
	mutable Node* hoveredNode = nullptr;
	mutable Port* connectingPort = nullptr;
	mutable std::pair<InputPort*, OutputPort*> droppedConnection;

	void drawNode(NodeDrawState& drawState) const;
	static void drawNodeTitleBar(NodeDrawState& drawState);
	void drawPort(PortDrawState& drawState) const;
	auto getNodeDrawState(Node* node) const -> NodeDrawState&;
	auto getPortDrawState(Port* port) const -> PortDrawState&;
};
} // namespace rsp::gui
