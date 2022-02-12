#pragma once

#include "clk/base/graph.hpp"
#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/viewer.hpp"
#include "clk/util/color_rgba.hpp"

#include <functional>
#include <imnodes.h>
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace clk::gui::impl
{
template <typename data, typename widget>
class widget_cache;
class node_editor;
class port_editor;

template <bool const_data>
class selection_manager;
} // namespace clk::gui::impl

namespace clk::gui
{
class graph_editor final : public editor_of<clk::graph>
{
public:
	graph_editor(std::shared_ptr<widget_factory const> factory, std::string_view name);
	graph_editor(graph_editor const&) = delete;
	graph_editor(graph_editor&&) = delete;
	auto operator=(graph_editor const&) = delete;
	auto operator=(graph_editor&&) = delete;
	~graph_editor() final;

	auto clone() const -> std::unique_ptr<widget> override;
	void copy(widget const& other) override;

	void center_view() const;

	auto draw_contents(clk::graph& graph) const -> bool final;

private:
	struct connection_change
	{
		clk::port& starting_port;
		clk::port* ending_port = nullptr;
		std::optional<std::pair<clk::port*, clk::port*>> dropped_connection = std::nullopt;
	};

	ImNodesEditorContext* _context = nullptr;
	std::unique_ptr<impl::widget_cache<clk::node, impl::node_editor>> _node_cache;
	std::unique_ptr<impl::widget_cache<clk::port, impl::port_editor>> _port_cache;
	mutable std::vector<std::pair<clk::input*, clk::output*>> _connections;
	std::unique_ptr<impl::selection_manager<false>> _selection_manager;
	mutable std::optional<connection_change> _new_connection_in_progress = std::nullopt;
	mutable std::optional<std::function<bool()>> _queued_action = std::nullopt;
	mutable bool _context_menu_queued = false;
	bool _draw_node_titles = true;
	bool _draw_port_widgets = true;

	void draw_graph(clk::graph& graph) const;
	void draw_menus(clk::graph& graph) const;
	void update_connections(clk::graph& graph) const;
	void handle_mouse_interactions(clk::graph& graph) const;
	void restore_dropped_connection() const;
};
} // namespace clk::gui
