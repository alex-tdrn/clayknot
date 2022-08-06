#include "clk/gui/widgets/graph_viewer.hpp"
#include "clk/base/graph.hpp"
#include "clk/base/input.hpp"
#include "clk/base/node.hpp"
#include "clk/base/output.hpp"
#include "clk/base/port.hpp"
#include "clk/gui/imgui_conversions.hpp"
#include "clk/gui/widgets/action_widget.hpp"
#include "clk/gui/widgets/widget.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/gui/widgets/widget_tree.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"
#include "imgui_guard.hpp"
#include "layout_solver.hpp"
#include "node_viewers.hpp"
#include "port_viewers.hpp"
#include "selection_manager.hpp"
#include "widget_cache.hpp"

#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <imnodes.h>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/any_view.hpp>
#include <range/v3/view/filter.hpp>

namespace clk::gui
{
graph_viewer::graph_viewer(std::shared_ptr<widget_factory const> factory, std::string_view name)
	: viewer_of<clk::graph>(std::move(factory), name)
	, _context(ImNodes::EditorContextCreate())
	, _node_cache(
		  std::make_unique<impl::widget_cache<clk::node const, impl::node_viewer>>([&](node const* node, int id) {
			  return impl::create_node_viewer(node, id, _port_cache.get(), _draw_node_titles);
		  }))
	, _port_cache(
		  std::make_unique<impl::widget_cache<clk::port const, impl::port_viewer>>([&](port const* port, int id) {
			  return impl::create_port_viewer(port, id, *get_widget_factory(), _draw_port_widgets);
		  }))
	, _selection_manager(std::make_unique<impl::selection_manager<true>>(_node_cache.get(), _port_cache.get()))
	, _layout_solver(std::make_unique<impl::layout_solver>())
{
	auto const& f = *get_widget_factory();
	settings().add(f.create(_draw_node_titles, "Draw node titles"));
	settings().add(f.create(_draw_port_widgets, "Draw port widgets"));
	settings().add(std::make_unique<action_widget>(
		[&]() {
			center_view();
		},
		"Center view"));

	{
		auto& layout_solver_settings = settings().get_subtree("Force based layout solver");
		layout_solver_settings.add(f.create(_enable_layout_solver, "Enabled"));
		_layout_solver->register_settings(layout_solver_settings, f);
	}

	disable_title();
	ImNodes::EditorContextSet(_context);
	ImNodes::EditorContextSet(nullptr);
}

graph_viewer::~graph_viewer()
{
	ImNodes::EditorContextFree(_context);
}

auto graph_viewer::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<graph_viewer>(this->get_widget_factory(), this->name());
	clone->copy(*this);
	return clone;
}

void graph_viewer::copy(widget const& other)
{
	viewer_of<clk::graph>::copy(other);
}

void graph_viewer::center_view() const
{
	_centering_queued = true;
}

void graph_viewer::draw_contents(clk::graph const& graph) const
{
	ImNodes::EditorContextSet(_context);
	ImNodes::PushStyleVar(ImNodesStyleVar_NodeCornerRounding, 0.0f);
	ImNodes::PushStyleVar(ImNodesStyleVar_PinOffset, ImNodes::GetStyle().PinHoverRadius * 0.5f);

	draw_graph(graph);
	_selection_manager->update();

	ImNodes::PopStyleVar();
	ImNodes::PopStyleVar();
	if(_first_draw)
	{
		_first_draw = false;
		ImNodes::EditorContextResetPanning(to_imgui(to_glm(ImGui::GetItemRectSize()) / 2.0f));
	}
	ImNodes::EditorContextSet(nullptr);
}

void graph_viewer::draw_graph(clk::graph const& graph) const
{
	_connections.clear();

	ImNodes::BeginNodeEditor();

	if(_centering_queued)
	{
		ImNodes::EditorContextResetPanning(
			{ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y / 2});
		_centering_queued = false;
	}

	if(!is_interactive())
	{
		auto* current_window = ImGui::GetCurrentWindow();
		ImGui::SetWindowHitTestHole(current_window, current_window->Pos, current_window->Size);
	}

	for(auto const& node : graph.nodes())
	{
		_node_cache->widget_for(node.get()).draw();
		for(auto* output : node->outputs())
			for(auto* input : output->connected_inputs())
				if(_port_cache->has_widget_for(input))
					_connections.emplace_back(std::make_pair(input, output));
	}

	{
		int link_id = 0;
		for(auto& connection : _connections)
		{
			imgui_guard link_style_guard;
			if(connection.first->is_faulty() || connection.second->is_faulty())
			{
				const float t = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(
					std::chrono::steady_clock::now().time_since_epoch())
									.count();
				const float f = (std::cos(t * 20.0f) + 1.0f) / 2.0f;
				auto c1 = color_rgba{1.0f, 0.0f, 0.0f, 1.0f};
				auto c2 = color_rgba{1.0f};
				auto error_color = (f * c1 + (1.0f - f) * c2).packed();
				link_style_guard.push_color_style(ImNodesCol_Link, error_color);
			}
			else
			{
				auto color = color_rgba(color_rgb::create_random(connection.first->data_type_hash()), 1.0f).packed();
				link_style_guard.push_color_style(ImNodesCol_Link, color);
			}

			ImNodes::Link(link_id++, _port_cache->widget_for(connection.first).id(),
				_port_cache->widget_for(connection.second).id());
		}
	}
	ImNodes::MiniMap(0.1f);

	if(_enable_layout_solver)
	{
		run_layout_solver(graph);
	}

	ImNodes::EndNodeEditor();
}

void graph_viewer::run_layout_solver(clk::graph const& graph) const
{
	_layout_solver->update_cache(graph, *_node_cache, *_port_cache);
	_layout_solver->step();
}

} // namespace clk::gui