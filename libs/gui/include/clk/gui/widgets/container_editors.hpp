#pragma once

#include "clk/gui/widgets/data_writer.hpp"
#include "editor.hpp"
#include "widget_factory.hpp"

#include <imgui.h>
#include <type_traits>
#include <vector>

namespace clk::gui
{

template <typename StoredDataType>
class vector_editor : public editor_of<std::vector<StoredDataType>>
{
public:
    using base = editor_of<std::vector<StoredDataType>>;

    vector_editor(std::shared_ptr<widget_factory const> factory, std::string_view name) : base(std::move(factory), name)
    {
        if constexpr(std::is_same_v<StoredDataType, bool>)
        {
            _stored_data_editor =
                base::get_widget_factory()->create(data_writer<bool>(
                                                       [this]() -> bool* {
                                                           static bool current_flag = true;
                                                           current_flag = (*_current_draw_data)[_current_draw_index];
                                                           return &current_flag;
                                                       },
                                                       [this](bool const* new_value) {
                                                           assert(_current_draw_data != nullptr);
                                                           (*_current_draw_data)[_current_draw_index] = *new_value;
                                                           _any_element_modified = true;
                                                       }),
                    "");
        }
        else
        {
            _stored_data_editor =
                base::get_widget_factory()->create(data_writer<StoredDataType>(
                                                       [this]() -> StoredDataType* {
                                                           assert(_current_draw_data != nullptr);
                                                           return &(*_current_draw_data)[_current_draw_index];
                                                       },
                                                       [this]() {
                                                           _any_element_modified = true;
                                                       }),
                    "");
        }
        _stored_data_editor->enable_extra_id();
        _stored_data_editor->disable_title();
    }

    vector_editor() = delete;
    vector_editor(vector_editor const&) = delete;
    vector_editor(vector_editor&&) = delete;
    auto operator=(vector_editor const&) -> vector_editor& = delete;
    auto operator=(vector_editor&&) -> vector_editor& = delete;
    ~vector_editor() override = default;

    auto clone() const -> std::unique_ptr<widget> override
    {
        auto clone = std::make_unique<vector_editor<StoredDataType>>(base::get_widget_factory(), base::name());
        clone->copy(*this);
        return clone;
    }

    auto draw_contents(std::vector<StoredDataType>& data) const -> bool override
    {
        _current_draw_data = &data;
        _any_element_modified = false;

        int insert_at = -1;
        int erase_at = -1;

        const float total_contents_height =
            ImGui::GetStyle().ItemSpacing.y * 4 + ImGui::GetTextLineHeightWithSpacing() +
            static_cast<float>(data.size()) * (_stored_data_editor->last_size().y + ImGui::GetStyle().ItemSpacing.y);

        ImGui::BeginChild(
            "vector scrollarea", ImVec2(base::available_width(), std::min(total_contents_height, 150.0f)), true);

        if(ImGui::SmallButton("+"))
        {
            insert_at = 0;
        }

        for(_current_draw_index = 0; _current_draw_index < data.size(); _current_draw_index++)
        {
            ImGui::PushID(static_cast<int>(_current_draw_index));

            ImGui::BeginGroup();
            if(ImGui::SmallButton("-"))
            {
                erase_at = static_cast<int>(_current_draw_index);
            }
            ImGui::SameLine();
            if(ImGui::SmallButton("+"))
            {
                insert_at = static_cast<int>(_current_draw_index + 1);
            }
            ImGui::SameLine();
            ImGui::Text("%li:", _current_draw_index);

            ImGui::EndGroup();

            ImGui::SameLine();
            _stored_data_editor->draw();

            ImGui::PopID();
        }

        if(insert_at != -1)
        {
            data.insert(data.begin() + insert_at, StoredDataType{});
        }
        else if(erase_at != -1)
        {
            data.erase(data.begin() + erase_at);
        }

        ImGui::EndChild();

        return _any_element_modified || insert_at != -1 || erase_at != -1;
    }

private:
    std::unique_ptr<editor> _stored_data_editor;
    mutable std::size_t _current_draw_index = 0;
    mutable std::vector<StoredDataType>* _current_draw_data = nullptr;
    mutable bool _any_element_modified = false;
};

} // namespace clk::gui
