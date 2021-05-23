#include "clk/gui/widgets/graph_editor.h"

#include "clk/algorithms/decompose_color.h"
#include "clk/algorithms/grayscale_color_node.h"
#include "clk/algorithms/mix_colors.h"
#include "clk/algorithms/random_color_source.h"
#include "clk/algorithms/value_to_color.h"
#include "clk/base/algorithm_node.hpp"
#include "clk/base/constant_node.hpp"
#include "clk/base/port.hpp"
#include "node_editors.hpp"
#include "port_editors.hpp"
#include "selection_manager.hpp"
#include "widget_cache.hpp"

namespace clk::gui
{
graph_editor::graph_editor(
	clk::graph* data, std::string const& data_name, std::optional<std::function<void()>> modified_callback)
	: editor_of<clk::graph>(data, data_name, std::move(modified_callback))
{
	_port_cache = std::make_unique<impl::widget_cache<port, impl::port_editor>>(&impl::create_port_editor);

	_node_cache = std::make_unique<impl::widget_cache<node, impl::node_editor>>([&](node* node, int id) {
		return impl::create_node_editor(node, id, _port_cache.get(), _modification_callback);
	});

	_selection_manager = std::make_unique<impl::selection_manager<false>>(_node_cache.get(), _port_cache.get());

	disable_title();
	_context = imnodes::EditorContextCreate();
	imnodes::EditorContextSet(_context);

	imnodes::EditorContextSet(nullptr);
}

graph_editor::~graph_editor()
{
	imnodes::EditorContextFree(_context);
}

auto graph_editor::clone() const -> std::unique_ptr<widget>
{
	return std::make_unique<graph_editor>(get_data(), get_data_name(), get_modified_callback());
}

void graph_editor::draw_contents() const
{
	imnodes::EditorContextSet(_context);

	draw_graph();
	draw_menus();
	update_connections();
	_selection_manager->update();
	handle_mouse_interactions();

	if(_modification_callback.has_value())
	{
		if((*_modification_callback)())
			_modification_callback = std::nullopt;
	}

	imnodes::EditorContextSet(nullptr);
}

void graph_editor::draw_graph() const
{
	imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkCreationOnSnap);
	if(_new_connection_in_progress)
		imnodes::PushColorStyle(
			imnodes::ColorStyle_Link, _port_cache->get_widget(&_new_connection_in_progress->starting_port).get_color());
	else
		imnodes::PushColorStyle(imnodes::ColorStyle_Link, clk::color_rgba{1.0f}.packed());

	_connections.clear();

	imnodes::BeginNodeEditor();

	for(auto const& node : *get_data())
	{
		_node_cache->get_widget(node.get()).draw();
		for(auto* output : node->get_output_ports())
			for(auto* input : output->get_connected_input_ports())
				if(_port_cache->has_widget(input))
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
			auto color =
				clk::color_rgba(clk::color_rgb::create_random(connection.first->get_data_type_hash()), 1.0f).packed();
			imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
			if(_new_connection_in_progress)
			{
				imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, color);
				imnodes::PushColorStyle(imnodes::ColorStyle_LinkSelected, color);
			}
			imnodes::Link(link_id++, _port_cache->get_widget(connection.first).get_id(),
				_port_cache->get_widget(connection.second).get_id());

			imnodes::PopColorStyle();
			if(_new_connection_in_progress)
			{
				imnodes::PopColorStyle();
				imnodes::PopColorStyle();
			}
		}
	}

	if(_new_connection_in_progress && _new_connection_in_progress->dropped_connection)
	{
		auto color = clk::color_rgba(clk::color_rgb(0.0f), 1.0f).packed();
		imnodes::PushColorStyle(imnodes::ColorStyle_Link, color);
		imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, color);
		imnodes::PushColorStyle(imnodes::ColorStyle_LinkSelected, color);

		imnodes::Link(-1, _port_cache->get_widget(_new_connection_in_progress->dropped_connection->first).get_id(),
			_port_cache->get_widget(_new_connection_in_progress->dropped_connection->second).get_id());

		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
	}
	imnodes::EndNodeEditor();
	imnodes::PopAttributeFlag();
	imnodes::PopColorStyle();
}

void graph_editor::draw_menus() const
{
	bool delet_this = false;
	if(ImGui::BeginPopupContextItem("Context menu"))
	{
		if(ImGui::BeginMenu("New node"))
		{
			auto* graph = get_data();
			if(ImGui::BeginMenu("Algorithm"))
			{
				if(ImGui::MenuItem("DecomposeColor"))
					graph->push_back(
						std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::decompose_color>()));
				if(ImGui::MenuItem("GrayscaleColorNode"))
					graph->push_back(std::make_unique<clk::algorithm_node>(
						std::make_unique<clk::algorithms::grayscale_color_node>()));
				if(ImGui::MenuItem("MixColors"))
					graph->push_back(
						std::make_unique<clk::algorithm_node>(std::make_unique<clk::algorithms::mix_colors>()));
				if(ImGui::MenuItem("RandomColorSource"))
					graph->push_back(std::make_unique<clk::algorithm_node>(
						std::make_unique<clk::algorithms::random_color_source>()));
				ImGui::EndMenu();
			}

			if(ImGui::MenuItem("Constant"))
				graph->push_back(std::make_unique<clk::constant_node>());
			ImGui::EndMenu();
		}

		if(imnodes::NumSelectedLinks() > 0 || imnodes::NumSelectedNodes() > 0)
		{
			ImGui::Separator();
			if(!_selection_manager->get_selected_nodes().empty())
			{
				const auto& nodes = _selection_manager->get_selected_nodes();

				bool any_inputs = std::any_of(nodes.begin(), nodes.end(), [](auto node) {
					return !node->get_input_ports().empty();
				});
				if(any_inputs && ImGui::MenuItem("Copy inputs to new constant node"))
				{
					auto constant_node = std::make_unique<clk::constant_node>();
					for(auto* node : _selection_manager->get_selected_nodes())
					{
						for(auto* input_port : node->get_input_ports())
							constant_node->add_port(std::unique_ptr<clk::output_port>(
								dynamic_cast<output_port*>(input_port->create_compatible_port().release())));
					}

					get_data()->push_back(std::move(constant_node));
				}
			}
			delet_this = ImGui::MenuItem("Delete");
		}
		ImGui::EndPopup();
	}

	if(delet_this || (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) &&
						 ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
	{
		if(imnodes::NumSelectedLinks() > 0)
		{
			std::vector<int> selectedLinks(imnodes::NumSelectedLinks());
			imnodes::GetSelectedLinks(selectedLinks.data());
			for(auto linkID : selectedLinks)
				_connections[linkID].first->disconnect_from(*_connections[linkID].second);
			imnodes::ClearLinkSelection();
		}

		auto* graph = get_data();

		for(auto* selectedNode : _selection_manager->get_selected_nodes())
		{
			graph->erase(std::remove_if(graph->begin(), graph->end(),
							 [&](auto const& node) {
								 return node.get() == selectedNode;
							 }),
				graph->end());
		}

		imnodes::ClearNodeSelection();
	}
}

void graph_editor::update_connections() const
{
	if(int connecting_port_id = -1; imnodes::IsLinkStarted(&connecting_port_id))
	{
		_new_connection_in_progress.emplace(connection_change{*_port_cache->get_widget(connecting_port_id).get_port()});

		for(const auto& it : _port_cache->get_map())
		{
			const auto& port_editor = it.second;

			port_editor->set_enabled(false);
			port_editor->set_stable_height(true);
			if(port_editor->get_port()->can_connect_to(_new_connection_in_progress->starting_port))
			{
				port_editor->set_enabled(true);
			}
		}
	}

	if(int output_port_id = -1, input_port_id = -1; imnodes::IsLinkCreated(&output_port_id, &input_port_id))
	{
		auto* input_port = dynamic_cast<clk::input_port*>(_port_cache->get_widget(input_port_id).get_port());
		auto* output_port = dynamic_cast<clk::output_port*>(_port_cache->get_widget(output_port_id).get_port());

		if(_new_connection_in_progress->ending_port != nullptr)
			_new_connection_in_progress->starting_port.disconnect_from(*_new_connection_in_progress->ending_port);

		if(input_port == &_new_connection_in_progress->starting_port)
			_new_connection_in_progress->ending_port = output_port;
		else
			_new_connection_in_progress->ending_port = input_port;

		restore_dropped_connection();

		if(input_port->is_connected())
			_new_connection_in_progress->dropped_connection =
				std::pair(input_port, input_port->get_connected_output_port());

		input_port->connect_to(*output_port);
	}

	if(int dummy = -1; _new_connection_in_progress && _new_connection_in_progress->ending_port != nullptr &&
					   !imnodes::IsPinHovered(&dummy))
	{
		_new_connection_in_progress->starting_port.disconnect_from(*_new_connection_in_progress->ending_port);
		_new_connection_in_progress->ending_port = nullptr;
		restore_dropped_connection();
	}
}

void graph_editor::handle_mouse_interactions() const
{
	if(ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		_new_connection_in_progress = std::nullopt;

		for(const auto& it : _port_cache->get_map())
		{
			const auto& portEditor = it.second;

			portEditor->set_enabled(true);
			portEditor->set_stable_height(false);
		}
	}

	if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && imnodes::NumSelectedLinks() > 0)
	{
		std::vector<int> selectedLinks(imnodes::NumSelectedLinks());
		imnodes::GetSelectedLinks(selectedLinks.data());
		for(auto linkID : selectedLinks)
			_connections[linkID].first->disconnect_from(*_connections[linkID].second);
		imnodes::ClearLinkSelection();
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