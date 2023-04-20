#pragma once

#include "clk/gui/widgets/data_reader.hpp"
#include "clk/gui/widgets/data_widget.hpp"

#include <imgui.h>
#include <typeindex>

namespace clk::gui
{
class viewer : public data_widget
{
public:
    using data_widget::data_widget;
    viewer() = delete;
    viewer(viewer const&) = delete;
    viewer(viewer&&) = delete;
    auto operator=(viewer const&) -> viewer& = delete;
    auto operator=(viewer&&) -> viewer& = delete;
    ~viewer() override = default;
};

template <typename DataType>
class viewer_of : public viewer
{
public:
    using viewer::viewer;
    viewer_of() = delete;
    viewer_of(viewer_of const&) = delete;
    viewer_of(viewer_of&&) = delete;
    auto operator=(viewer_of const&) -> viewer_of& = delete;
    auto operator=(viewer_of&&) -> viewer_of& = delete;
    ~viewer_of() override = default;

    auto clone() const -> std::unique_ptr<widget> override
    {
        auto clone = std::make_unique<viewer_of<DataType>>(get_widget_factory(), name());
        clone->copy(*this);
        return clone;
    }

    void copy(widget const& other) override
    {
        auto const& casted = dynamic_cast<viewer_of<DataType> const&>(other);
        _data = casted._data;
        viewer::copy(other);
    }

    auto data_type_hash() const -> std::size_t override
    {
        static std::size_t hash = std::type_index(typeid(DataType)).hash_code();
        return hash;
    }

    void set_data_reader(data_reader<DataType> data)
    {
        _data = std::move(data);
    }

    virtual void draw_contents(DataType const& /*data*/) const
    {
        ImGui::Text("NO VIEWER IMPLEMENTATION");
    }

private:
    data_reader<DataType> _data;

    void draw_contents() const final
    {
        if(auto const* data = _data.read(); data != nullptr)
        {
            draw_contents(*data);
        }
        else
        {
            ImGui::Text("NO DATA");
        }
    }
};

template <>
class viewer_of<void> : public viewer
{
public:
    using viewer::viewer;
    viewer_of() = delete;
    viewer_of(viewer_of const&) = delete;
    viewer_of(viewer_of&&) = delete;
    auto operator=(viewer_of const&) -> viewer_of& = delete;
    auto operator=(viewer_of&&) -> viewer_of& = delete;
    ~viewer_of() override = default;

    auto clone() const -> std::unique_ptr<widget> override
    {
        auto clone = std::make_unique<viewer_of<void>>(get_widget_factory(), name());
        clone->copy(*this);
        return clone;
    }

    void copy(widget const& other) override
    {
        viewer::copy(other);
    }

    auto data_type_hash() const -> std::size_t override
    {
        static std::size_t hash = std::type_index(typeid(void)).hash_code();
        return hash;
    }

private:
    void draw_contents() const final
    {
        ImGui::Text("VOID VIEWER");
    }
};

} // namespace clk::gui
