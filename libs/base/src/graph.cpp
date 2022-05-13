#include "clk/base/graph.hpp"
#include "clk/base/port.hpp"
#include "clk/util/predicates.hpp"
#include "clk/util/projections.hpp"

#include <range/v3/algorithm/remove_if.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/any_view.hpp>
#include <utility>

namespace clk
{

void graph::add_node(std::unique_ptr<clk::node>&& node)
{
	for(auto* port : node->all_ports())
		port->set_connection_changed_callback([this]() {
			_timestamp.update();
		});

	_nodes.push_back(std::move(node));
	_timestamp.update();
}

void graph::remove_node(clk::node* node)
{
	_nodes.erase(
		ranges::remove_if(_nodes, clk::predicates::is_equal_to(node), clk::projections::underlying()), _nodes.end());
	_timestamp.update();
}

auto graph::nodes() const -> std::vector<std::unique_ptr<clk::node>> const&
{
	return _nodes;
}

auto graph::timestamp() const -> clk::timestamp
{
	return _timestamp;
}
} // namespace clk