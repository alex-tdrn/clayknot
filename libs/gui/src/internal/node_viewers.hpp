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

	auto id() const -> int;
	auto node() const -> clk::node const*;
	void set_highlighted(bool highlighted);
	void draw();

private:
	widget_cache<clk::port const, port_viewer>* _port_cache = nullptr;
	clk::node const* _node = nullptr;
	int _id = -1;
	bool _first_draw = true;
	float _title_width = 0;
	float _contents_width = 0;
	bool _highlighted = false;

	void draw_title_bar();
};

inline node_viewer::node_viewer(clk::node const* node, int id, widget_cache<clk::port const, port_viewer>* port_cache)
	: _port_cache(port_cache), _node(node), _id(id)
{
}

inline auto node_viewer::id() const -> int
{
	return _id;
}

inline auto node_viewer::node() const -> clk::node const*
{
	return _node;
}

inline void node_viewer::set_highlighted(bool highlighted)
{
	_highlighted = highlighted;
}

inline void node_viewer::draw()
{
	if(_highlighted)
		imnodes::PushColorStyle(imnodes::ColorStyle_NodeOutline, color_rgba{1.0f}.packed());

	imnodes::BeginNode(_id);

	draw_title_bar();

	ImGui::BeginGroup();
	for(auto* port : _node->inputs())
		_port_cache->widget_for(port).draw();
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	for(auto* port : _node->outputs())
		_port_cache->widget_for(port).draw();
	ImGui::EndGroup();

	imnodes::EndNode();
	_contents_width = ImGui::GetItemRectSize().x;

	if(_highlighted)
		imnodes::PopColorStyle();

	_first_draw = false;
}

inline void node_viewer::draw_title_bar()
{
	imnodes::BeginNodeTitleBar();

	ImGui::BeginGroup();

	if(!_first_draw && _title_width < _contents_width)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (_contents_width - _title_width) / 2);

	ImGui::Text("%s", _node->name().data());

	ImGui::EndGroup();

	if(_first_draw)
		_title_width = ImGui::GetItemRectSize().x;

	imnodes::EndNodeTitleBar();
}

inline auto create_node_viewer(clk::node const* node, int id, widget_cache<clk::port const, port_viewer>* portCache)
	-> std::unique_ptr<node_viewer>
{
	return std::make_unique<node_viewer>(node, id, portCache);
}

} // namespace clk::gui::impl