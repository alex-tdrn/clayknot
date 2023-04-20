#pragma once

#include <imnodes.h>

namespace clk::gui
{

class imgui_guard
{
public:
    imgui_guard() = default;
    imgui_guard(imgui_guard const&) = default;
    imgui_guard(imgui_guard&&) = default;
    auto operator=(imgui_guard const&) -> imgui_guard& = default;
    auto operator=(imgui_guard&&) -> imgui_guard& = default;

    ~imgui_guard()
    {
        for(int i = 0; i < _count_color_style; i++)
        {
            ImNodes::PopColorStyle();
        }

        for(int i = 0; i < _count_style_var; i++)
        {
            ImNodes::PopStyleVar();
        }
    }

    void push_color_style(ImNodesCol item, unsigned int color)
    {
        ImNodes::PushColorStyle(item, color);
        _count_color_style++;
    }

    void pop_color_style()
    {
        if(_count_color_style > 0)
        {
            ImNodes::PopColorStyle();
            _count_color_style--;
        }
    }

    void push_style_var(ImNodesStyleVar style_item, float value)
    {
        ImNodes::PushStyleVar(style_item, value);
        _count_style_var++;
    }

    void pop_style_var()
    {
        if(_count_style_var > 0)
        {
            ImNodes::PopStyleVar();
            _count_style_var--;
        }
    }

private:
    int _count_color_style = 0;
    int _count_style_var = 0;
};

} // namespace clk::gui
