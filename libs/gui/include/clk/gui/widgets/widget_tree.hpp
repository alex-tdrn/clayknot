#pragma once

#include "clk/gui/widgets/widget.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace clk::gui
{

class widget_tree : public widget
{
public:
    enum class draw_mode
    {
        tree_nodes,
        menu
    };

    using widget::widget;
    widget_tree() = delete;
    widget_tree(widget_tree const&) = delete;
    widget_tree(widget_tree&&) = delete;
    auto operator=(widget_tree const&) -> widget_tree& = delete;
    auto operator=(widget_tree&&) -> widget_tree& = delete;
    ~widget_tree() override = default;

    auto clone() const -> std::unique_ptr<widget> override;
    void copy(widget const& other) override;

    auto get_subtree(std::string_view path) -> widget_tree&;
    void add(std::unique_ptr<widget> widget);
    void set_draw_mode(draw_mode mode);

    void draw_contents() const override;

private:
    std::vector<std::unique_ptr<widget_tree>> _subtrees;
    std::vector<std::unique_ptr<widget>> _widgets;
    draw_mode _draw_mode = draw_mode::tree_nodes;
    bool _draw_inline = true;
};

} // namespace clk::gui
