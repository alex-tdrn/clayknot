#include "clk/gui/widgets/graph_viewer.hpp"

#include "clk/base/node.hpp"
#include "clk/base/port.hpp"
#include "node_viewers.hpp"
#include "port_viewers.hpp"
#include "selection_manager.hpp"
#include "widget_cache.hpp"

#include <imgui_internal.h>

namespace clk::gui
{
graph_viewer::graph_viewer(clk::graph const* data, std::string_view data_name)
	: viewer_of<clk::graph>(data, data_name)
	, _node_cache(
		  std::make_unique<impl::widget_cache<clk::node const, impl::node_viewer>>([&](node const* node, int id) {
			  return impl::create_node_viewer(node, id, _port_cache.get());
		  }))
	, _port_cache(std::make_unique<impl::widget_cache<clk::port const, impl::port_viewer>>(&impl::create_port_viewer))
	, _selection_manager(std::make_unique<impl::selection_manager<true>>(_node_cache.get(), _port_cache.get()))
{
	disable_title();
	_context = ImNodes::EditorContextCreate();
	ImNodes::EditorContextSet(_context);

	ImNodes::EditorContextSet(nullptr);
}

graph_viewer::~graph_viewer()
{
	ImNodes::EditorContextFree(_context);
}

auto graph_viewer::clone() const -> std::unique_ptr<widget>
{
	return std::make_unique<graph_viewer>(data(), data_name());
}

void graph_viewer::draw_contents() const
{
	ImNodes::EditorContextSet(_context);
	ImNodes::PushStyleVar(ImNodesStyleVar_NodeCornerRounding, 0.0f);
	ImNodes::PushStyleVar(ImNodesStyleVar_PinOffset, ImNodes::GetStyle().PinHoverRadius * 0.5f);

	draw_graph();
	_selection_manager->update();

	ImNodes::PopStyleVar();
	ImNodes::PopStyleVar();
	ImNodes::EditorContextSet(nullptr);
}

void graph_viewer::draw_graph() const
{
	_connections.clear();

	ImNodes::BeginNodeEditor();

	if(!is_interactive())
	{
		auto* current_window = ImGui::GetCurrentWindow();
		ImGui::SetWindowHitTestHole(current_window, current_window->Pos, current_window->Size);
	}

	for(auto const& node : *data())
	{
		_node_cache->widget_for(node.get()).draw();
		for(auto* output : node->outputs())
			for(auto* input : output->connected_inputs())
				if(_port_cache->has_widget_for(input))
					_connections.emplace_back(std::make_pair(input, output));
	}

	{
		int linkID = 0;
		for(auto& connection : _connections)
		{
			auto color = color_rgba(color_rgb::create_random(connection.first->data_type_hash()), 1.0f).packed();
			ImNodes::PushColorStyle(ImNodesCol_Link, color);
			ImNodes::Link(linkID++, _port_cache->widget_for(connection.first).id(),
				_port_cache->widget_for(connection.second).id());
			ImNodes::PopColorStyle();
		}
	}
	ImNodes::MiniMap(0.1f);

	ImNodes::EndNodeEditor();
}

} // namespace clk::gui