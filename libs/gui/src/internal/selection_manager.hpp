#pragma once

#include "node_editors.hpp"
#include "node_viewers.hpp"
#include "port_editors.hpp"
#include "port_viewers.hpp"
#include "widget_cache.hpp"

#include <imnodes.h>
#include <type_traits>
#include <unordered_set>

namespace rsp::gui::impl
{
template <bool const_data>
class selection_manager
{
public:
	using node_type = std::conditional_t<const_data, rsp::node const, rsp::node>;
	using port_type = std::conditional_t<const_data, rsp::port const, rsp::port>;
	using node_widget = std::conditional_t<const_data, node_viewer, node_editor>;
	using port_widget = std::conditional_t<const_data, port_viewer, port_editor>;

	selection_manager() = delete;
	selection_manager(
		widget_cache<node_type, node_widget>* node_cache, widget_cache<port_type, port_widget>* port_cache);
	selection_manager(selection_manager const&) = delete;
	selection_manager(selection_manager&&) = delete;
	auto operator=(selection_manager const&) -> selection_manager& = delete;
	auto operator=(selection_manager&&) -> selection_manager& = delete;
	~selection_manager() = default;

	void update();
	auto get_selected_nodes() const -> std::unordered_set<node*> const&;

private:
	widget_cache<node_type, node_widget>* node_cache;
	widget_cache<port_type, port_widget>* port_cache;
	std::unordered_set<node_type*> selected_nodes;
	node_type* hovered_node = nullptr; // TODO add hovered pins and connection as well (why not?)
};

template <bool const_data>
selection_manager<const_data>::selection_manager(
	widget_cache<node_type, node_widget>* node_cache, widget_cache<port_type, port_widget>* port_cache)
	: node_cache(node_cache), port_cache(port_cache)
{
}

template <bool const_data>
inline void selection_manager<const_data>::update()
{
	if(imnodes::NumSelectedNodes() != static_cast<int>(selected_nodes.size()) || selected_nodes.size() == 1)
	{
		for(auto* node : selected_nodes)
			node_cache->get_widget(node).set_highlighted(false);

		selected_nodes.clear();
		if(imnodes::NumSelectedNodes() > 0)
		{
			std::vector<int> selected_node_ids(imnodes::NumSelectedNodes());
			imnodes::GetSelectedNodes(selected_node_ids.data());

			for(auto node_id : selected_node_ids)
			{
				selected_nodes.insert(node_cache->get_widget(node_id).get_node());
			}
		}

		for(auto* node : selected_nodes)
			node_cache->get_widget(node).set_highlighted(true);
	}

	{
		node_type* new_hovered_node = nullptr;
		int hovered_node_id = -1;
		if(imnodes::IsNodeHovered(&hovered_node_id))
		{
			auto& new_hovered_node_viewer = node_cache->get_widget(hovered_node_id);
			new_hovered_node_viewer.set_highlighted(true);
			new_hovered_node = new_hovered_node_viewer.get_node();
		}

		if(hovered_node != nullptr && new_hovered_node != hovered_node &&
			selected_nodes.find(hovered_node) == selected_nodes.end())
			node_cache->get_widget(hovered_node).set_highlighted(false);

		hovered_node = new_hovered_node;
	}
}

template <bool const_data>
inline auto selection_manager<const_data>::get_selected_nodes() const -> std::unordered_set<node*> const&
{
	return selected_nodes;
}

} // namespace rsp::gui::impl
