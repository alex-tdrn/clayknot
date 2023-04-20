#pragma once

#include "clk/gui/widgets/data_widget.hpp"
#include "clk/gui/widgets/data_writer.hpp"

#include <imgui.h>
#include <typeindex>

namespace clk::gui
{
class editor : public data_widget
{
public:
    using data_widget::data_widget;
    editor() = delete;
    editor(editor const&) = delete;
    editor(editor&&) = delete;
    auto operator=(editor const&) -> editor& = delete;
    auto operator=(editor&&) -> editor& = delete;
    ~editor() override = default;
};

template <typename DataType>
class editor_of : public editor
{
public:
    using editor::editor;
    editor_of() = delete;
    editor_of(editor_of const&) = delete;
    editor_of(editor_of&&) = delete;
    auto operator=(editor_of const&) -> editor_of& = delete;
    auto operator=(editor_of&&) -> editor_of& = delete;
    ~editor_of() override = default;

    auto clone() const -> std::unique_ptr<widget> override
    {
        auto clone = std::make_unique<editor_of<DataType>>(get_widget_factory(), name());
        clone->copy(*this);
        return clone;
    }

    void copy(widget const& other) override
    {
        auto const& casted = dynamic_cast<editor_of<DataType> const&>(other);
        _data = casted._data;
        editor::copy(other);
    }

    auto data_type_hash() const -> std::size_t override
    {
        static std::size_t hash = std::type_index(typeid(DataType)).hash_code();
        return hash;
    }

    void set_data_writer(data_writer<DataType> data)
    {
        _data = std::move(data);
    }

    virtual auto draw_contents(DataType& /*data*/) const -> bool
    {
        ImGui::Text("NO EDITOR IMPLEMENTATION");
        return false;
    }

private:
    data_writer<DataType> _data;

    void draw_contents() const final
    {
        if(auto* data = _data.read(); data != nullptr)
        {
            if(draw_contents(*data))
            {
                _data.write(data); // TODO pass in new data
            }
        }
        else
        {
            ImGui::Text("NO DATA");
        }
    }
};

template <>
class editor_of<void> : public editor
{
public:
    using editor::editor;
    editor_of() = delete;
    editor_of(editor_of const&) = delete;
    editor_of(editor_of&&) = delete;
    auto operator=(editor_of const&) -> editor_of& = delete;
    auto operator=(editor_of&&) -> editor_of& = delete;
    ~editor_of() override = default;

    auto clone() const -> std::unique_ptr<widget> override
    {
        auto clone = std::make_unique<editor_of<void>>(get_widget_factory(), name());
        clone->copy(*this);
        return clone;
    }

    void copy(widget const& other) override
    {
        editor::copy(other);
    }

    auto data_type_hash() const -> std::size_t override
    {
        static std::size_t hash = std::type_index(typeid(void)).hash_code();
        return hash;
    }

private:
    void draw_contents() const final
    {
        ImGui::Text("VOID EDITOR");
    }
};

} // namespace clk::gui
