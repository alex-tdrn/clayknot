#pragma once
#include <memory>

namespace clk
{
class node;
class port;
} // namespace clk

namespace clk::gui::impl
{
class port_viewer;
template <typename DataType, typename Widget>
class widget_cache;
} // namespace clk::gui::impl

namespace clk::gui::impl
{

class node_viewer
{
public:
    node_viewer() = delete;
    node_viewer(clk::node const* node, int id, widget_cache<clk::port const, port_viewer>* port_cache,
        bool const& draw_node_titles);
    node_viewer(node_viewer const&) = delete;
    node_viewer(node_viewer&&) noexcept = delete;
    auto operator=(node_viewer const&) -> node_viewer& = delete;
    auto operator=(node_viewer&&) noexcept -> node_viewer& = delete;
    ~node_viewer() = default;

    auto id() const -> int;
    auto node() const -> clk::node const*;
    void set_highlighted(bool highlighted);
    void draw();

private:
    widget_cache<clk::port const, port_viewer>* _port_cache = nullptr;
    clk::node const* _node = nullptr;
    int _id = -1;
    bool _first_draw = true;
    float _title_width = 0;
    float _contents_width = 0;
    bool _highlighted = false;
    bool const& _draw_node_titles;

    void draw_title_bar();
};

auto create_node_viewer(clk::node const* node, int id, widget_cache<clk::port const, port_viewer>* port_cache,
    bool const& draw_node_titles) -> std::unique_ptr<node_viewer>;

} // namespace clk::gui::impl
