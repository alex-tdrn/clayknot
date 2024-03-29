#include "port_viewers.hpp"
#include "clk/base/port.hpp"
#include "clk/gui/imgui_conversions.hpp"
#include "clk/gui/widgets/data_reader.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"
#include "imgui_guard.hpp"
#include "port_color.hpp"

#include <imgui.h>
#include <imnodes.h>

namespace clk::gui::impl
{

port_viewer::port_viewer(
    clk::port const* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
    : _id(id)
    , _data_viewer(widget_factory.create(data_reader<void>{[=]() {
        return port->data_pointer();
    }},
          port->data_type_hash(), port->name()))
    , _draw_port_widgets(draw_port_widgets)
{
    _data_viewer->set_maximum_width(200);
}

auto port_viewer::id() const -> int
{
    return _id;
}

auto port_viewer::position() const -> glm::vec2 const&
{
    return _position;
}

void port_viewer::update_viewer_type()
{
    if(_data_viewer->data_type_hash() != port()->data_type_hash())
    {
        _data_viewer = _data_viewer->get_widget_factory()->create(data_reader<void>{[=]() {
            return port()->data_pointer();
        }},
            port()->data_type_hash(), port()->name());
        _data_viewer->set_maximum_width(200);
    }
}

input_viewer::input_viewer(
    clk::input const* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
    : port_viewer(port, id, widget_factory, draw_port_widgets), _port(port)
{
}

auto input_viewer::port() const -> clk::input const*
{
    return _port;
}

void input_viewer::draw()
{
    imgui_guard style_guard;

    if(_port->is_faulty())
    {
        const float t = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(
            std::chrono::steady_clock::now().time_since_epoch())
                            .count();
        const float f = (std::cos(t * 20.0f) + 1.0f) / 2.0f;
        auto c1 = color_rgba{1.0f, 0.0f, 0.0f, 1.0f};
        auto c2 = color_rgba{1.0f};
        auto error_color = (f * c1 + (1.0f - f) * c2).packed();

        style_guard.push_color_style(ImNodesCol_Pin, error_color);
        style_guard.push_color_style(ImNodesCol_PinHovered, error_color);
    }
    else
    {
        style_guard.push_color_style(ImNodesCol_Pin, port_color(_port));
        style_guard.push_color_style(ImNodesCol_PinHovered, port_color(_port));
    }

    ImNodes::BeginInputAttribute(_id, ImNodesPinShape_QuadFilled);

    if(_draw_port_widgets)
    {
        update_viewer_type();
        _data_viewer->draw();
    }
    else
    {
        ImGui::Dummy(ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()));
    }

    ImNodes::EndInputAttribute();
    auto rect_min = to_glm(ImGui::GetItemRectMin());
    auto rect_max = to_glm(ImGui::GetItemRectMax());
    _position.y = (rect_min.y + rect_max.y) / 2;
    _position.x = rect_min.x;
}

output_viewer::output_viewer(
    clk::output const* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
    : port_viewer(port, id, widget_factory, draw_port_widgets), _port(port)
{
}

auto output_viewer::port() const -> output const*
{
    return _port;
}

void output_viewer::draw()
{
    imgui_guard style_guard;

    if(_port->is_faulty())
    {
        const float t = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(
            std::chrono::steady_clock::now().time_since_epoch())
                            .count();
        const float f = (std::cos(t * 20.0f) + 1.0f) / 2.0f;
        auto c1 = color_rgba{1.0f, 0.0f, 0.0f, 1.0f};
        auto c2 = color_rgba{1.0f};
        auto error_color = (f * c1 + (1.0f - f) * c2).packed();

        style_guard.push_color_style(ImNodesCol_Pin, error_color);
        style_guard.push_color_style(ImNodesCol_PinHovered, error_color);
    }
    else
    {
        style_guard.push_color_style(ImNodesCol_Pin, port_color(_port));
        style_guard.push_color_style(ImNodesCol_PinHovered, port_color(_port));
    }

    ImNodes::BeginOutputAttribute(_id, ImNodesPinShape_TriangleFilled);

    if(_draw_port_widgets)
    {
        update_viewer_type();
        _data_viewer->draw();
    }
    else
    {
        ImGui::Dummy(ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()));
    }

    ImNodes::EndOutputAttribute();
    auto rect_min = to_glm(ImGui::GetItemRectMin());
    auto rect_max = to_glm(ImGui::GetItemRectMax());
    _position.y = (rect_min.y + rect_max.y) / 2;
    _position.x = rect_max.x;
}

auto create_port_viewer(clk::port const* port, int id, widget_factory const& widget_factory,
    bool const& draw_port_widgets) -> std::unique_ptr<port_viewer>
{
    if(auto const* input_port = dynamic_cast<clk::input const*>(port); input_port != nullptr)
        return std::make_unique<input_viewer>(input_port, id, widget_factory, draw_port_widgets);
    else if(auto const* output_port = dynamic_cast<clk::output const*>(port); output_port != nullptr)
        return std::make_unique<output_viewer>(output_port, id, widget_factory, draw_port_widgets);
    return nullptr;
}

} // namespace clk::gui::impl
