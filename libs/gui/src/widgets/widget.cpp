#include "clk/gui/widgets/widget.hpp"

#include "clk/gui/imgui_conversions.hpp"
#include "clk/gui/widgets/widget_tree.hpp"

#include <imgui.h>

namespace clk::gui
{

widget::widget(std::string_view name) : _name(name)
{
}

widget::~widget() = default;

void widget::copy(widget const& other)
{
    _name = other._name;
    _draw_title = other._draw_title;
    _push_extra_id = other._push_extra_id;
    _interactive = other._interactive;
    _maximum_width = other._maximum_width;
    _last_size = other._last_size;
    _extended_available = other._extended_available;
    _extended_preferred = other._extended_preferred;
}

void widget::draw() const
{
    if(_push_extra_id)
        ImGui::PushID(this);
    if(_extended_available && ImGui::BeginPopup(_name.c_str()))
    {
        bool was_extended_preferred = _extended_preferred;
        std::optional<float> old_maximum_width = _maximum_width;

        _extended_preferred = true;
        _maximum_width = ImGui::GetIO().DisplaySize.y * 0.75f;

        if(_draw_title)
            ImGui::Text("%s", _name.c_str());
        draw_contents();
        ImGui::EndPopup();

        _extended_preferred = was_extended_preferred;
        _maximum_width = old_maximum_width;
    }
    else
    {
        ImGui::BeginGroup();
        if(_draw_title)
            ImGui::Text("%s", _name.c_str());
        if(_extended_available)
        {
            if(_draw_title)
                ImGui::SameLine();
            if(ImGui::SmallButton((_extended_preferred ? "-" : "+")))
                _extended_preferred = !_extended_preferred;
            if(_extended_preferred)
            {
                ImGui::SameLine();
                if(ImGui::SmallButton("+"))
                    ImGui::OpenPopup(_name.c_str());
            }
        }
        draw_contents();
        ImGui::EndGroup();
        _last_size = to_glm(ImGui::GetItemRectSize());
    }
    if(_push_extra_id)
        ImGui::PopID();
    _first_draw = false;
}

void widget::enable_title()
{
    _draw_title = true;
}

void widget::disable_title()
{
    _draw_title = false;
}

void widget::enable_extra_id()
{
    _push_extra_id = true;
}

void widget::disable_extra_id()
{
    _push_extra_id = false;
}

void widget::set_interactivity(bool interactive) noexcept
{
    _interactive = interactive;
}

auto widget::is_interactive() const noexcept -> bool
{
    return _interactive;
}

void widget::prefer_extended()
{
    _extended_available = true;
    _extended_preferred = true;
}

void widget::prefer_compact()
{
    _extended_available = true;
    _extended_preferred = false;
}

void widget::set_maximum_width(float width)
{
    _maximum_width = width;
}

void widget::clear_maximum_width()
{
    _maximum_width.reset();
}

auto widget::last_size() const -> glm::vec2
{
    return _last_size;
}

void widget::set_name(std::string_view name)
{
    _name = name;
}

auto widget::name() const -> std::string_view
{
    return _name;
}

auto widget::get_settings() const -> widget_tree const*
{
    return _settings.get();
}

auto widget::is_first_draw() const -> bool
{
    return _first_draw;
}

auto widget::available_width() const -> float
{
    if(_maximum_width)
        return _maximum_width.value();
    return ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().WindowPadding.x -
           ImGui::GetStyle().FramePadding.x;
}

auto widget::extended_preferred() const -> bool
{
    _extended_available = true;
    return _extended_preferred;
}

auto widget::settings() -> widget_tree&
{
    if(_settings == nullptr)
    {
        _settings = std::make_unique<widget_tree>("Settings for '" + _name + "'");
        _settings->disable_title();
        _settings->set_draw_mode(widget_tree::draw_mode::menu);
    }
    return *_settings;
}
} // namespace clk::gui
