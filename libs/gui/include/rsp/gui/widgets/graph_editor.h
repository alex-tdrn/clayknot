#pragma once

#include "rsp/base/graph.hpp"
#include "rsp/gui/widgets/editor.hpp"
#include "rsp/gui/widgets/viewer.hpp"
#include "rsp/util/color_rgba.hpp"

#include <functional>
#include <imnodes.h>
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace rsp::gui::impl
{
template <typename data, typename widget>
class widget_cache;
class node_editor;
class port_editor;

template <bool const_data>
class selection_manager;
} // namespace rsp::gui::impl

namespace rsp::gui
{
class graph_editor final : public editor_of<rsp::graph>
{
public:
	graph_editor() = delete;
	graph_editor(rsp::graph* data, std::string const& data_name,
		std::optional<std::function<void()>> modified_callback = std::nullopt);
	graph_editor(graph_editor const&) = delete;
	graph_editor(graph_editor&&) = delete;
	auto operator=(graph_editor const&) = delete;
	auto operator=(graph_editor&&) = delete;
	~graph_editor() final;

	auto clone() const -> std::unique_ptr<rsp::gui::widget> final;
	void draw_contents() const final;

private:
	struct connection_change
	{
		rsp::port& starting_port;
		rsp::port* ending_port = nullptr;
		std::optional<std::pair<rsp::port*, rsp::port*>> dropped_connection = std::nullopt;
	};

	imnodes::EditorContext* context;
	std::unique_ptr<impl::widget_cache<rsp::node, impl::node_editor>> node_cache;
	std::unique_ptr<impl::widget_cache<rsp::port, impl::port_editor>> port_cache;
	mutable std::vector<std::pair<rsp::input_port*, rsp::output_port*>> connections;
	std::unique_ptr<impl::selection_manager<false>> selection_manager;
	mutable std::optional<connection_change> new_connection_in_progress = std::nullopt;
	mutable std::optional<std::function<bool()>> modification_callback = std::nullopt; // this is smelly af

	void draw_graph() const;
	void draw_menus() const;
	void update_connections() const;
	void handle_mouse_interactions() const;
	void restore_dropped_connection() const;
};
} // namespace rsp::gui
