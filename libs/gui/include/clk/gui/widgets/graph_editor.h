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
	graph_editor() = delete;
	graph_editor(clk::graph* data, std::string const& data_name,
		std::optional<std::function<void()>> modified_callback = std::nullopt);
	graph_editor(graph_editor const&) = delete;
	graph_editor(graph_editor&&) = delete;
	auto operator=(graph_editor const&) = delete;
	auto operator=(graph_editor&&) = delete;
	~graph_editor() final;

	auto clone() const -> std::unique_ptr<clk::gui::widget> final;
	void draw_contents() const final;

private:
	struct connection_change
	{
		clk::port& starting_port;
		clk::port* ending_port = nullptr;
		std::optional<std::pair<clk::port*, clk::port*>> dropped_connection = std::nullopt;
	};

	imnodes::EditorContext* context;
	std::unique_ptr<impl::widget_cache<clk::node, impl::node_editor>> node_cache;
	std::unique_ptr<impl::widget_cache<clk::port, impl::port_editor>> port_cache;
	mutable std::vector<std::pair<clk::input_port*, clk::output_port*>> connections;
	std::unique_ptr<impl::selection_manager<false>> selection_manager;
	mutable std::optional<connection_change> new_connection_in_progress = std::nullopt;
	mutable std::optional<std::function<bool()>> modification_callback = std::nullopt; // this is smelly af

	void draw_graph() const;
	void draw_menus() const;
	void update_connections() const;
	void handle_mouse_interactions() const;
	void restore_dropped_connection() const;
};
} // namespace clk::gui
