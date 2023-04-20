#pragma once

#include "clk/gui/widgets/viewer.hpp"
#include "node_viewers.hpp"
#include "port_viewers.hpp"

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

struct ImNodesEditorContext;

namespace clk
{
class graph;
class input;
class node;
class output;
class port;
} // namespace clk

namespace clk::gui::impl
{
template <typename Data, typename Widget>
class widget_cache;
template <bool ConstData>
class selection_manager;
class layout_solver;
} // namespace clk::gui::impl

namespace clk::gui
{
class widget;
class widget_factory;
class graph_viewer final : public viewer_of<clk::graph>
{
public:
    graph_viewer(std::shared_ptr<widget_factory const> factory, std::string_view name);
    graph_viewer(graph_viewer const&) = delete;
    graph_viewer(graph_viewer&&) = delete;
    auto operator=(graph_viewer const&) -> graph_viewer& = delete;
    auto operator=(graph_viewer&&) -> graph_viewer& = delete;
    ~graph_viewer() final;

    auto clone() const -> std::unique_ptr<widget> override;
    void copy(widget const& other) override;

    void center_view() const;

    void draw_contents(clk::graph const& graph) const final;

private:
    mutable bool _first_draw = true;
    ImNodesEditorContext* _context;
    std::unique_ptr<impl::widget_cache<clk::node const, impl::node_viewer>> _node_cache;
    std::unique_ptr<impl::widget_cache<clk::port const, impl::port_viewer>> _port_cache;
    mutable std::vector<std::pair<clk::input const*, clk::output const*>> _connections;
    std::unique_ptr<impl::selection_manager<true>> _selection_manager;
    std::unique_ptr<impl::layout_solver> _layout_solver;
    bool _draw_port_widgets = true;
    bool _draw_node_titles = true;
    bool _enable_layout_solver = true;
    mutable bool _centering_queued = true;

    void draw_graph(clk::graph const& graph) const;
    void run_layout_solver(clk::graph const& graph) const;
};

} // namespace clk::gui
