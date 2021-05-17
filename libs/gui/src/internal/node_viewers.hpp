#pragma once
#include "clk/base/node.hpp"
#include "clk/util/color_rgba.hpp"
#include "port_viewers.hpp"
#include "widget_cache.hpp"


#include <imgui.h>
#include <imnodes.h>

namespace clk::gui::impl
{
class node_viewer
{
public:
	node_viewer() = delete;
	node_viewer(clk::node const* node, int id, widget_cache<clk::port const, port_viewer>* port_cache);
	node_viewer(node_viewer const&) = delete;
	node_viewer(node_viewer&&) noexcept = delete;
	auto operator=(node_viewer const&) -> node_viewer& = delete;
	auto operator=(node_viewer&&) noexcept -> node_viewer& = delete;
	~node_viewer() = default;

	auto get_id() const -> int;
	auto get_node() const -> clk::node const*;
	void set_highlighted(bool highlighted);
	void draw();

private:
	widget_cache<clk::port const, port_viewer>* port_cache = nullptr;
	clk::node const* node = nullptr;
	int id = -1;
	bool first_draw = true;
	float title_width = 0;
	float contents_width = 0;
	bool highlighted = false;

	void draw_title_bar();
};

inline node_viewer::node_viewer(clk::node const* node, int id, widget_cache<clk::port const, port_viewer>* port_cache)
	: port_cache(port_cache), node(node), id(id)
{
}

inline auto node_viewer::get_id() const -> int
{
	return id;
}

inline auto node_viewer::get_node() const -> clk::node const*
{
	return node;
}

inline void node_viewer::set_highlighted(bool highlighted)
{
	this->highlighted = highlighted;
}

inline void node_viewer::draw()
{
	if(highlighted)
		imnodes::PushColorStyle(imnodes::ColorStyle_NodeOutline, color_rgba{1.0f}.packed());

	imnodes::BeginNode(id);

	draw_title_bar();

	ImGui::BeginGroup();
	for(auto& port : node->get_input_ports())
		port_cache->get_widget(port).draw();
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	for(auto& port : node->get_output_ports())
		port_cache->get_widget(port).draw();
	ImGui::EndGroup();

	imnodes::EndNode();
	contents_width = ImGui::GetItemRectSize().x;

	if(highlighted)
		imnodes::PopColorStyle();

	first_draw = false;
}

inline void node_viewer::draw_title_bar()
{
	imnodes::BeginNodeTitleBar();

	ImGui::BeginGroup();

	if(!first_draw && title_width < contents_width)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (contents_width - title_width) / 2);

	ImGui::Text("%s", node->get_name().data());

	ImGui::EndGroup();

	if(first_draw)
		title_width = ImGui::GetItemRectSize().x;

	imnodes::EndNodeTitleBar();
}

inline auto create_node_viewer(clk::node const* node, int id, widget_cache<clk::port const, port_viewer>* portCache)
	-> std::unique_ptr<node_viewer>
{
	return std::make_unique<node_viewer>(node, id, portCache);
}

} // namespace clk::gui::impl