#pragma once

#include "clk/base/node.hpp"
#include "clk/util/timestamp.hpp"

#include <memory>
#include <vector>

namespace clk
{
class graph final
{
public:
	graph() = default;
	graph(graph const&) = delete;
	graph(graph&&) = default;
	auto operator=(graph const&) -> graph& = delete;
	auto operator=(graph&&) -> graph& = default;
	~graph() = default;

	void add_node(std::unique_ptr<clk::node>&& node);
	void remove_node(clk::node* node);
	auto nodes() const -> std::vector<std::unique_ptr<clk::node>> const&;
	auto timestamp() const -> clk::timestamp;

private:
	clk::timestamp _timestamp;
	std::vector<std::unique_ptr<clk::node>> _nodes;
};

inline void graph::add_node(std::unique_ptr<clk::node>&& node)
{
	for(auto* port : node->all_ports())
		port->set_connection_changed_callback([this]() {
			_timestamp.update();
		});

	_nodes.push_back(std::move(node));
	_timestamp.update();
}

inline void graph::remove_node(clk::node* node)
{
	_nodes.erase(
		ranges::remove_if(_nodes, clk::predicates::is_equal_to(node), clk::projections::underlying()), _nodes.end());
	_timestamp.update();
}

inline auto graph::nodes() const -> std::vector<std::unique_ptr<clk::node>> const&
{
	return _nodes;
}

inline auto graph::timestamp() const -> clk::timestamp
{
	return _timestamp;
}

} // namespace clk