#include "node_viewers.hpp"
#include "clk/base/input.hpp"
#include "clk/base/node.hpp"
#include "clk/base/output.hpp"
#include "clk/base/port.hpp"
#include "clk/util/color_rgba.hpp"
#include "port_viewers.hpp"
#include "widget_cache.hpp"

#include <imgui.h>
#include <imnodes.h>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/any_view.hpp>
#include <string_view>

namespace clk::gui::impl
{

node_viewer::node_viewer(
	clk::node const* node, int id, widget_cache<clk::port const, port_viewer>* port_cache, bool const& draw_node_titles)
	: _port_cache(port_cache), _node(node), _id(id), _draw_node_titles(draw_node_titles)
{
}

auto node_viewer::id() const -> int
{
	return _id;
}

auto node_viewer::node() const -> clk::node const*
{
	return _node;
}

void node_viewer::set_highlighted(bool highlighted)
{
	_highlighted = highlighted;
}

void node_viewer::draw()
{
	if(_highlighted)
		ImNodes::PushColorStyle(ImNodesCol_NodeOutline, color_rgba{1.0f}.packed());

	ImNodes::BeginNode(_id);

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

	ImNodes::EndNode();
	_contents_width = ImGui::GetItemRectSize().x;

	if(_highlighted)
		ImNodes::PopColorStyle();

	_first_draw = false;
}

void node_viewer::draw_title_bar()
{
	ImNodes::BeginNodeTitleBar();

	ImGui::BeginGroup();

	if(!_first_draw && _title_width < _contents_width)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (_contents_width - _title_width) / 2);

	if(_draw_node_titles)
	{
		ImGui::Text("%s", _node->name().data());
	}

	ImGui::EndGroup();

	if(_first_draw)
		_title_width = ImGui::GetItemRectSize().x;

	ImNodes::EndNodeTitleBar();
}

auto create_node_viewer(clk::node const* node, int id, widget_cache<clk::port const, port_viewer>* port_cache,
	bool const& draw_node_titles) -> std::unique_ptr<node_viewer>
{
	return std::make_unique<node_viewer>(node, id, port_cache, draw_node_titles);
}

} // namespace clk::gui::impl