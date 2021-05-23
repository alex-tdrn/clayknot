#pragma once

#include "node_editors.hpp"
#include "node_viewers.hpp"
#include "port_editors.hpp"
#include "port_viewers.hpp"
#include "widget_cache.hpp"

#include <imnodes.h>
#include <type_traits>
#include <unordered_set>

namespace clk::gui::impl
{
template <bool const_data>
class selection_manager
{
public:
	using node_type = std::conditional_t<const_data, clk::node const, clk::node>;
	using port_type = std::conditional_t<const_data, clk::port const, clk::port>;
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
	widget_cache<node_type, node_widget>* _node_cache;
	widget_cache<port_type, port_widget>* _port_cache;
	std::unordered_set<node_type*> _selected_nodes;
	node_type* _hovered_node = nullptr; // TODO add hovered pins and connection as well (why not?)
};

template <bool const_data>
selection_manager<const_data>::selection_manager(
	widget_cache<node_type, node_widget>* node_cache, widget_cache<port_type, port_widget>* port_cache)
	: _node_cache(node_cache), _port_cache(port_cache)
{
}

template <bool const_data>
inline void selection_manager<const_data>::update()
{
	if(imnodes::NumSelectedNodes() != static_cast<int>(_selected_nodes.size()) || _selected_nodes.size() == 1)
	{
		for(auto* node : _selected_nodes)
			_node_cache->get_widget(node).set_highlighted(false);

		_selected_nodes.clear();
		if(imnodes::NumSelectedNodes() > 0)
		{
			std::vector<int> selected_node_ids(imnodes::NumSelectedNodes());
			imnodes::GetSelectedNodes(selected_node_ids.data());

			for(auto node_id : selected_node_ids)
			{
				_selected_nodes.insert(_node_cache->get_widget(node_id).get_node());
			}
		}

		for(auto* node : _selected_nodes)
			_node_cache->get_widget(node).set_highlighted(true);
	}

	{
		node_type* new_hovered_node = nullptr;
		int hovered_node_id = -1;
		if(imnodes::IsNodeHovered(&hovered_node_id))
		{
			auto& new_hovered_node_viewer = _node_cache->get_widget(hovered_node_id);
			new_hovered_node_viewer.set_highlighted(true);
			new_hovered_node = new_hovered_node_viewer.get_node();
		}

		if(_hovered_node != nullptr && new_hovered_node != _hovered_node &&
			_selected_nodes.find(_hovered_node) == _selected_nodes.end())
			_node_cache->get_widget(_hovered_node).set_highlighted(false);

		_hovered_node = new_hovered_node;
	}
}

template <bool const_data>
inline auto selection_manager<const_data>::get_selected_nodes() const -> std::unordered_set<node*> const&
{
	return _selected_nodes;
}

} // namespace clk::gui::impl
