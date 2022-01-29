#pragma once

#include "clk/base/graph.hpp"
#include "clk/gui/widgets/viewer.hpp"
#include "clk/gui/widgets/widget.hpp"

#include <imnodes.h>

namespace clk::gui::impl
{
template <typename data, typename widget>
class widget_cache;
class node_viewer;
class port_viewer;

template <bool const_data>
class selection_manager;
} // namespace clk::gui::impl

namespace clk::gui
{
class graph_viewer final : public viewer_of<clk::graph>
{
public:
	graph_viewer(std::shared_ptr<widget_factory> factory, std::string_view name);
	graph_viewer(graph_viewer const&) = delete;
	graph_viewer(graph_viewer&&) = delete;
	auto operator=(graph_viewer const&) -> graph_viewer& = delete;
	auto operator=(graph_viewer&&) -> graph_viewer& = delete;
	~graph_viewer() final;

	auto clone() const -> std::unique_ptr<widget> override;
	void copy(widget const& other) override;

	void draw_contents(clk::graph const& graph) const final;

private:
	ImNodesEditorContext* _context;
	std::unique_ptr<impl::widget_cache<clk::node const, impl::node_viewer>> _node_cache;
	std::unique_ptr<impl::widget_cache<clk::port const, impl::port_viewer>> _port_cache;
	mutable std::vector<std::pair<clk::input const*, clk::output const*>> _connections;
	std::unique_ptr<impl::selection_manager<true>> _selection_manager;
	bool _draw_port_widgets = true;
	bool _draw_node_titles = true;

	void draw_graph(clk::graph const& graph) const;
};

} // namespace clk::gui