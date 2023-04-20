#include "clk/gui/widgets/action_widget.hpp"

#include <imgui.h>
#include <utility>

namespace clk::gui
{
action_widget::action_widget(std::function<void()> action, std::string_view name)
    : widget(name), _action(std::move(action))
{
    disable_title();
}

auto action_widget::clone() const -> std::unique_ptr<widget>
{
    auto clone = std::make_unique<action_widget>(this->_action, this->name());
    clone->copy(*this);
    return clone;
}

void action_widget::copy(widget const& other)
{
    widget::copy(other);
}

void action_widget::draw_contents() const
{
    if(!_action)
    {
        ImGui::Text("NO ACTION SET");
    }
    else if(ImGui::Button(name().data()))
    {
        _action();
    }
}
} // namespace clk::gui
