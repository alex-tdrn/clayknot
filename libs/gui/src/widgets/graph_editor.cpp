#include "clk/gui/widgets/graph_editor.hpp"

#include "clk/algorithms/color.hpp"
#include "clk/base/algorithm_node.hpp"
#include "clk/base/constant_node.hpp"
#include "clk/base/port.hpp"
#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/widget_setting.hpp"
#include "clk/util/predicates.hpp"
#include "clk/util/projections.hpp"
#include "node_editors.hpp"
#include "port_editors.hpp"
#include "selection_manager.hpp"
#include "widget_cache.hpp"

#include <range/v3/algorithm.hpp>

#include <imgui_internal.h>

namespace clk::gui
{
graph_editor::graph_editor(std::shared_ptr<widget_factory> factory, std::string_view name)
	: editor_of<clk::graph>(std::move(factory), name)
	, _context(ImNodes::EditorContextCreate())
	, _node_cache(std::make_unique<impl::widget_cache<node, impl::node_editor>>([&](node* node, int id) {
		return impl::create_node_editor(
			node, id, _port_cache.get(), _queued_action, *get_widget_factory(), _draw_node_titles);
	}))
	, _port_cache(std::make_unique<impl::widget_cache<port, impl::port_editor>>([&](port* port, int id) {
		return impl::create_port_editor(port, id, *get_widget_factory(), _draw_port_widgets);
	}))
	, _selection_manager(std::make_unique<impl::selection_manager<false>>(_node_cache.get(), _port_cache.get()))

{
	register_setting(_draw_node_titles, "Draw node titles");
	register_setting(_draw_port_widgets, "Draw port widgets");
	disable_title();
	ImNodes::EditorContextSet(_context);
	ImNodes::EditorContextSet(nullptr);
}

graph_editor::~graph_editor()
{
	ImNodes::EditorContextFree(_context);
}

auto graph_editor::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<graph_editor>(this->get_widget_factory(), this->name());
	clone->copy(*this);
	return clone;
}

void graph_editor::copy(widget const& other)
{
	editor_of<clk::graph>::copy(other);
}

auto graph_editor::draw_contents(clk::graph& graph) const -> bool
{
	auto last_timestamp = graph.timestamp().time_point();
	auto time_since_last_modification = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(
		std::chrono::steady_clock::now() - last_timestamp);

	ImGui::Text("Last modified: %.1fs", time_since_last_modification.count());

	ImNodes::EditorContextSet(_context);
	ImNodes::PushStyleVar(ImNodesStyleVar_NodeCornerRounding, 0.0f);
	ImNodes::PushStyleVar(ImNodesStyleVar_PinOffset, ImNodes::GetStyle().PinHoverRadius * 0.75f);
	draw_graph(graph);
	draw_menus(graph);
	update_connections(graph);
	_selection_manager->update();
	handle_mouse_interactions(graph);

	if(_queued_action.has_value())
	{
		if((*_queued_action)())
			_queued_action = std::nullopt;
	}

	ImNodes::PopStyleVar();
	ImNodes::PopStyleVar();
	ImNodes::EditorContextSet(nullptr);

	return last_timestamp != graph.timestamp().time_point();
}

void graph_editor::draw_graph(clk::graph& graph) const
{
	ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkCreationOnSnap);
	if(_new_connection_in_progress)
		ImNodes::PushColorStyle(
			ImNodesCol_Link, _port_cache->widget_for(&_new_connection_in_progress->starting_port).color());
	else
		ImNodes::PushColorStyle(ImNodesCol_Link, clk::color_rgba{1.0f}.packed());

	_connections.clear();

	ImNodes::BeginNodeEditor();

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
			if(_new_connection_in_progress && _new_connection_in_progress->ending_port != nullptr)
			{
				auto snap_connection =
					std::pair(&_new_connection_in_progress->starting_port, _new_connection_in_progress->ending_port);

				if((snap_connection.first == connection.first && snap_connection.second == connection.second) ||
					(snap_connection.first == connection.second && snap_connection.second == connection.first))
				{
					link_id++;
					continue;
				}
			}
			float link_opacity = 1.0f;
			if(_new_connection_in_progress)
				link_opacity = 0.25f;
			auto color =
				clk::color_rgba(clk::color_rgb::create_random(connection.first->data_type_hash()), link_opacity)
					.packed();
			ImNodes::PushColorStyle(ImNodesCol_Link, color);
			if(_new_connection_in_progress)
			{
				ImNodes::PushColorStyle(ImNodesCol_LinkHovered, color);
				ImNodes::PushColorStyle(ImNodesCol_LinkSelected, color);
			}
			ImNodes::Link(link_id++, _port_cache->widget_for(connection.first).id(),
				_port_cache->widget_for(connection.second).id());

			ImNodes::PopColorStyle();
			if(_new_connection_in_progress)
			{
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
			}
		}
	}

	if(_new_connection_in_progress && _new_connection_in_progress->dropped_connection)
	{
		auto color = clk::color_rgba(clk::color_rgb(0.0f), 1.0f).packed();
		ImNodes::PushColorStyle(ImNodesCol_Link, color);
		ImNodes::PushColorStyle(ImNodesCol_LinkHovered, color);
		ImNodes::PushColorStyle(ImNodesCol_LinkSelected, color);

		ImNodes::Link(-1, _port_cache->widget_for(_new_connection_in_progress->dropped_connection->first).id(),
			_port_cache->widget_for(_new_connection_in_progress->dropped_connection->second).id());

		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
	}

	ImNodes::MiniMap(0.1f);
	_context_menu_queued = ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImNodes::IsEditorHovered();

	ImNodes::EndNodeEditor();
	ImNodes::PopAttributeFlag();
	ImNodes::PopColorStyle();
}

void graph_editor::draw_menus(clk::graph& graph) const
{
	bool delet_this = false;
	if(_context_menu_queued)
	{
		ImGui::OpenPopup("Context Menu");
		_context_menu_queued = false;
	}

	if(ImGui::BeginPopup("Context Menu"))
	{
		if(ImGui::BeginMenu("New node"))
		{
			std::unique_ptr<clk::node> new_node = nullptr;
			if(ImGui::BeginMenu("Algorithm"))
			{
				for(auto [algorithm_name, algorithm_factory] : clk::algorithm::factories())
					if(ImGui::MenuItem(algorithm_name.c_str()))
					{
						new_node = std::make_unique<algorithm_node>(algorithm_factory());
					}

				ImGui::EndMenu();
			}

			if(ImGui::MenuItem("Constant"))
				new_node = std::make_unique<clk::constant_node>();

			if(new_node != nullptr)
			{
				if(!_node_cache->has_widget_for(new_node.get()))
					ImNodes::SetNodeScreenSpacePos(_node_cache->widget_for(new_node.get()).id(), ImGui::GetMousePos());
				graph.add_node(std::move(new_node));
			}
			ImGui::EndMenu();
		}

		if(ImNodes::NumSelectedLinks() > 0 || ImNodes::NumSelectedNodes() > 0)
		{
			ImGui::Separator();
			if(!_selection_manager->selected_nodes().empty())
			{
				const auto& nodes = _selection_manager->selected_nodes();

				bool any_inputs = ranges::any_of(nodes, [](auto node) {
					return node->has_inputs();
				});
				if(any_inputs && ImGui::MenuItem("Copy inputs to new constant node"))
				{
					auto constant_node = std::make_unique<clk::constant_node>();
					for(auto* node : _selection_manager->selected_nodes())
					{
						for(auto* input : node->inputs())
							constant_node->add_output(std::unique_ptr<clk::output>(
								dynamic_cast<output*>(input->create_compatible_port().release())));
					}

					graph.add_node(std::move(constant_node));
				}
			}
			delet_this = ImGui::MenuItem("Delete");
		}
		ImGui::EndPopup();
	}

	if(delet_this || (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) &&
						 ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
	{
		if(ImNodes::NumSelectedLinks() > 0)
		{
			std::vector<int> selectedLinks(ImNodes::NumSelectedLinks());
			ImNodes::GetSelectedLinks(selectedLinks.data());
			for(auto linkID : selectedLinks)
				_connections[linkID].first->disconnect_from(*_connections[linkID].second);
			ImNodes::ClearLinkSelection();
		}

		for(auto* selected_node : _selection_manager->selected_nodes())
			graph.remove_node(selected_node);

		ImNodes::ClearNodeSelection();
	}
}

void graph_editor::update_connections(clk::graph& graph) const
{
	if(int connecting_port_id = -1; ImNodes::IsLinkStarted(&connecting_port_id))
	{
		_new_connection_in_progress.emplace(connection_change{*_port_cache->widget_for(connecting_port_id).port()});

		for(auto const& node : graph.nodes())
		{
			for(auto* port : node->all_ports())
			{
				auto& port_editor = _port_cache->widget_for(port);

				port_editor.set_enabled(false);
				port_editor.set_stable_height(true);
				if(port->can_connect_to(_new_connection_in_progress->starting_port) ||
					port == &_new_connection_in_progress->starting_port)
				{
					port_editor.set_enabled(true);
				}
			}
		}
	}

	if(int output_id = -1, input_id = -1; ImNodes::IsLinkCreated(&output_id, &input_id))
	{
		auto* input = dynamic_cast<clk::input*>(_port_cache->widget_for(input_id).port());
		auto* output = dynamic_cast<clk::output*>(_port_cache->widget_for(output_id).port());

		if(_new_connection_in_progress->ending_port != nullptr)
			_new_connection_in_progress->starting_port.disconnect_from(*_new_connection_in_progress->ending_port);

		if(input == &_new_connection_in_progress->starting_port)
			_new_connection_in_progress->ending_port = output;
		else
			_new_connection_in_progress->ending_port = input;

		restore_dropped_connection();

		if(input->is_connected())
			_new_connection_in_progress->dropped_connection = std::pair(input, input->connected_output());

		input->connect_to(*output);
	}

	if(int dummy = -1; _new_connection_in_progress && _new_connection_in_progress->ending_port != nullptr &&
					   !ImNodes::IsPinHovered(&dummy))
	{
		_new_connection_in_progress->starting_port.disconnect_from(*_new_connection_in_progress->ending_port);
		_new_connection_in_progress->ending_port = nullptr;
		restore_dropped_connection();
	}
}

void graph_editor::handle_mouse_interactions(clk::graph& graph) const
{
	if(ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		_new_connection_in_progress = std::nullopt;

		for(auto const& node : graph.nodes())
		{
			for(auto* port : node->all_ports())
			{
				auto& port_editor = _port_cache->widget_for(port);

				port_editor.set_enabled(true);
				port_editor.set_stable_height(false);
			}
		}
	}

	if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImNodes::NumSelectedLinks() > 0)
	{
		std::vector<int> selectedLinks(ImNodes::NumSelectedLinks());
		ImNodes::GetSelectedLinks(selectedLinks.data());
		for(auto linkID : selectedLinks)
			_connections[linkID].first->disconnect_from(*_connections[linkID].second);
		ImNodes::ClearLinkSelection();
	}
}

void graph_editor::restore_dropped_connection() const
{
	if(_new_connection_in_progress && _new_connection_in_progress->dropped_connection)
	{
		_new_connection_in_progress->dropped_connection->first->connect_to(
			*_new_connection_in_progress->dropped_connection->second);
		_new_connection_in_progress->dropped_connection = std::nullopt;
	}
}

} // namespace clk::gui