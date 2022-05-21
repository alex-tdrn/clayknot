#pragma once

#include "clk/gui/widgets/data_writer.hpp"
#include "editor.hpp"
#include "widget_factory.hpp"

#include <imgui.h>
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
													   [this, current_flag = bool(true)]() mutable -> bool* {
														   assert(_current_draw_data != nullptr);
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
		_stored_data_editor->disable_extra_id();
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
		for(std::size_t i = 0; i < data.size(); i++)
		{
			_current_draw_index = i;
			ImGui::Text("%li: ", i);
			ImGui::PushID(_current_draw_index);
			ImGui::SameLine();
			_stored_data_editor->draw();
			ImGui::PopID();
		}
		return _any_element_modified;
	}

private:
	std::unique_ptr<editor> _stored_data_editor;
	mutable std::size_t _current_draw_index = 0;
	mutable std::vector<StoredDataType>* _current_draw_data = nullptr;
	mutable bool _any_element_modified = false;
};

} // namespace clk::gui