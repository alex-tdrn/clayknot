#pragma once

#include "rsp/base/graph.hpp"
#include "rsp/gui/widgets/viewer.hpp"

#include <imnodes.h>

namespace rsp::gui::impl
{
template <typename data, typename widget>
class widget_cache;
class node_viewer;
class port_viewer;

template <bool const_data>
class selection_manager;
} // namespace rsp::gui::impl

namespace rsp::gui
{
class graph_viewer final : public viewer_of<rsp::graph>
{
public:
	graph_viewer() = delete;
	graph_viewer(rsp::graph const* data, std::string const& data_name);
	graph_viewer(graph_viewer const&) = delete;
	graph_viewer(graph_viewer&&) = delete;
	auto operator=(graph_viewer const&) -> graph_viewer& = delete;
	auto operator=(graph_viewer&&) -> graph_viewer& = delete;
	~graph_viewer() final;

	auto clone() const -> std::unique_ptr<rsp::gui::widget> final;
	void draw_contents() const final;

private:
	imnodes::EditorContext* context;
	std::unique_ptr<impl::widget_cache<rsp::node const, impl::node_viewer>> node_cache;
	std::unique_ptr<impl::widget_cache<rsp::port const, impl::port_viewer>> port_cache;
	mutable std::vector<std::pair<rsp::input_port const*, rsp::output_port const*>> connections;
	std::unique_ptr<impl::selection_manager<true>> selection_manager;

	void draw_graph() const;
};

} // namespace rsp::gui