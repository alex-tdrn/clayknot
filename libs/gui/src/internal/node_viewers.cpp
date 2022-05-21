#include "node_viewers.hpp"
#include "clk/base/input.hpp"
#include "clk/base/node.hpp"
#include "clk/base/output.hpp"
#include "clk/base/port.hpp"
#include "clk/util/color_rgba.hpp"
#include "port_viewers.hpp"
#include "widget_cache.hpp"

#include <bits/chrono.h>
#include <chrono>
#include <cmath>
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
	auto const& error_message = _node->error();

	if(!error_message.empty())
	{
		const float t = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(
			std::chrono::steady_clock::now().time_since_epoch())
							.count();
		const float f = (std::cos(t * 20.0f) + 1.0f) / 2.0f;
		auto c1 = color_rgba{1.0f, 0.0f, 0.0f, 1.0f};
		auto c2 = color_rgba{1.0f};

		ImNodes::PushColorStyle(ImNodesCol_NodeOutline, (f * c1 + (1.0f - f) * c2).packed());
		ImNodes::PushStyleVar(ImNodesStyleVar_NodeBorderThickness, 2.0f);
	}
	else if(_highlighted)
	{
		ImNodes::PushColorStyle(ImNodesCol_NodeOutline, color_rgba{1.0f}.packed());
	}

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

	if(!error_message.empty())
	{
		ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, "%s", error_message.c_str());
	}

	ImNodes::EndNode();
	_contents_width = ImGui::GetItemRectSize().x;

	if(!error_message.empty() || _highlighted)
	{
		ImNodes::PopColorStyle();
		if(!error_message.empty())
		{
			ImNodes::PopStyleVar();
		}
	}

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