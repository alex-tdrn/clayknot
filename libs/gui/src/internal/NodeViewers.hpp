#pragma once
#include "PortViewers.hpp"
#include "WidgetCache.hpp"
#include "rsp/base/Node.hpp"
#include "rsp/util/ColorRGBA.hpp"

#include <imgui.h>
#include <imnodes.h>

namespace rsp::gui::impl
{
class NodeViewer
{
public:
	NodeViewer() = delete;
	NodeViewer(Node const* node, int id, WidgetCache<Port const, PortViewer>* portCache);
	NodeViewer(NodeViewer const&) = delete;
	NodeViewer(NodeViewer&&) noexcept = delete;
	auto operator=(NodeViewer const&) -> NodeViewer& = delete;
	auto operator=(NodeViewer&&) noexcept -> NodeViewer& = delete;
	~NodeViewer() = default;

	auto getID() const -> int;
	auto getNode() const -> Node const*;
	void setHighlighted(bool highlighted);
	void draw();

private:
	WidgetCache<Port const, PortViewer>* portCache = nullptr;
	Node const* node = nullptr;
	int id = -1;
	bool firstDraw = true;
	float titleWidth = 0;
	float contentsWidth = 0;
	bool highlighted = false;

	void drawTitleBar();
};

inline NodeViewer::NodeViewer(Node const* node, int id, WidgetCache<Port const, PortViewer>* portCache)
	: portCache(portCache), node(node), id(id)
{
}

inline auto NodeViewer::getID() const -> int
{
	return id;
}

inline auto NodeViewer::getNode() const -> Node const*
{
	return node;
}

inline void NodeViewer::setHighlighted(bool highlighted)
{
	this->highlighted = highlighted;
}

inline void NodeViewer::draw()
{
	if(highlighted)
		imnodes::PushColorStyle(imnodes::ColorStyle_NodeOutline, ColorRGBA{1.0f}.packed());

	imnodes::BeginNode(id);

	drawTitleBar();

	ImGui::BeginGroup();
	for(auto& port : node->getInputPorts())
		portCache->getWidget(port).draw();
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	for(auto& port : node->getOutputPorts())
		portCache->getWidget(port).draw();
	ImGui::EndGroup();

	imnodes::EndNode();
	contentsWidth = ImGui::GetItemRectSize().x;

	if(highlighted)
		imnodes::PopColorStyle();

	firstDraw = false;
}

inline void NodeViewer::drawTitleBar()
{
	imnodes::BeginNodeTitleBar();

	ImGui::BeginGroup();

	if(!firstDraw && titleWidth < contentsWidth)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (contentsWidth - titleWidth) / 2);

	ImGui::Text("%s", node->getName().data());

	ImGui::EndGroup();

	if(firstDraw)
		titleWidth = ImGui::GetItemRectSize().x;

	imnodes::EndNodeTitleBar();
}

inline auto createNodeViewer(Node const* node, int id, WidgetCache<Port const, PortViewer>* portCache)
	-> std::unique_ptr<NodeViewer>
{
	return std::make_unique<NodeViewer>(node, id, portCache);
}

} // namespace rsp::gui::impl