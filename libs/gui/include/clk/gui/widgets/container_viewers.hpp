#pragma once

#include "clk/gui/widgets/data_reader.hpp"
#include "viewer.hpp"
#include "widget_factory.hpp"

#include <imgui.h>
#include <vector>

namespace clk::gui
{

template <typename StoredDataType>
class vector_viewer : public viewer_of<std::vector<StoredDataType>>
{
public:
	using base = viewer_of<std::vector<StoredDataType>>;

	vector_viewer(std::shared_ptr<widget_factory const> factory, std::string_view name) : base(std::move(factory), name)
	{
		if constexpr(std::is_same_v<StoredDataType, bool>)
		{
			_stored_data_viewer = base::get_widget_factory()->create(data_reader<bool>([this]() -> bool const* {
				static bool current_flag = true;
				current_flag = (*_current_draw_data)[_current_draw_index];
				return &current_flag;
			}),
				"");
		}
		else
		{
			_stored_data_viewer =
				base::get_widget_factory()->create(data_reader<StoredDataType>([this]() -> StoredDataType const* {
					assert(_current_draw_data != nullptr);
					return &(*_current_draw_data)[_current_draw_index];
				}),
					"");
		}
		_stored_data_viewer->enable_extra_id();
		_stored_data_viewer->disable_title();
	}

	vector_viewer() = delete;
	vector_viewer(vector_viewer const&) = delete;
	vector_viewer(vector_viewer&&) = delete;
	auto operator=(vector_viewer const&) -> vector_viewer& = delete;
	auto operator=(vector_viewer&&) -> vector_viewer& = delete;
	~vector_viewer() override = default;

	auto clone() const -> std::unique_ptr<widget> override
	{
		auto clone = std::make_unique<vector_viewer<StoredDataType>>(base::get_widget_factory(), base::name());
		clone->copy(*this);
		return clone;
	}

	void draw_contents(std::vector<StoredDataType> const& data) const override
	{
		_current_draw_data = &data;

		const float total_contents_height =
			ImGui::GetStyle().ItemSpacing.y * 4 +
			static_cast<float>(data.size()) * (_stored_data_viewer->last_size().y + ImGui::GetStyle().ItemSpacing.y);

		ImGui::BeginChild(
			"vector scrollarea", ImVec2(base::available_width(), std::min(total_contents_height, 150.0f)), true);

		for(std::size_t i = 0; i < data.size(); i++)
		{
			_current_draw_index = i;
			ImGui::Text("%li: ", i);
			ImGui::PushID(_current_draw_index);
			ImGui::SameLine();
			_stored_data_viewer->draw();
			ImGui::PopID();
		}

		ImGui::EndChild();
	}

private:
	std::unique_ptr<viewer> _stored_data_viewer;
	mutable std::size_t _current_draw_index = 0;
	mutable std::vector<StoredDataType> const* _current_draw_data = nullptr;
};

} // namespace clk::gui