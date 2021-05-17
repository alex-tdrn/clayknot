#pragma once

#include <imgui.h>
#include <memory>
#include <optional>
#include <string>

namespace clk::gui
{
class widget
{
public:
	widget() = delete;
	explicit widget(std::string data_name);
	widget(widget const&) = delete;
	widget(widget&&) = delete;
	auto operator=(widget const&) -> widget& = delete;
	auto operator=(widget&&) -> widget& = delete;
	virtual ~widget() = default;

	virtual auto clone() const -> std::unique_ptr<widget> = 0;
	void draw() const;
	void enable_title();
	void disable_title();
	void prefer_extended();
	void prefer_compact();
	void set_maximum_width(float width);
	void clear_maximum_width();
	auto get_last_size() const -> glm::vec2;
	auto get_data_name() const -> std::string const&;

protected:
	auto get_available_width() const -> float;
	auto is_extended_preferred() const -> bool;
	virtual auto is_data_available() const noexcept -> bool = 0;
	virtual void draw_contents() const = 0;

private:
	std::string const data_name;
	bool draw_title = true;
	mutable std::optional<float> maximum_width;
	mutable glm::vec2 last_size = {0.0f, 0.0f};
	mutable bool extended_available = false;
	mutable bool extended_preferred = false;
};

inline widget::widget(std::string data_name) : data_name(std::move(data_name))
{
}

inline void widget::draw() const
{
	ImGui::PushID(this);
	if(ImGui::BeginPopup(data_name.c_str()))
	{
		bool was_extended_preferred = extended_preferred;
		std::optional<float> old_maximum_width = maximum_width;

		extended_preferred = true;
		maximum_width = ImGui::GetIO().DisplaySize.y * 0.75;

		if(draw_title)
			ImGui::Text("%s", data_name.c_str());
		if(is_data_available())
			draw_contents();
		ImGui::EndPopup();

		extended_preferred = was_extended_preferred;
		maximum_width = old_maximum_width;
	}
	else
	{
		ImGui::BeginGroup();
		if(draw_title)
			ImGui::Text("%s", data_name.c_str());
		if(extended_available)
		{
			if(draw_title)
				ImGui::SameLine();
			if(ImGui::SmallButton((extended_preferred ? "-" : "+")))
				extended_preferred = !extended_preferred;
			if(extended_preferred)
			{
				ImGui::SameLine();
				if(ImGui::SmallButton("+"))
					ImGui::OpenPopup(data_name.c_str());
			}
		}
		if(is_data_available())
			draw_contents();
		else
			ImGui::Text("No data...");
		ImGui::EndGroup();
		last_size = ImGui::GetItemRectSize();
	}
	ImGui::PopID();
}

inline void widget::enable_title()
{
	draw_title = true;
}

inline void widget::disable_title()
{
	draw_title = false;
}

inline void widget::prefer_extended()
{
	extended_available = true;
	extended_preferred = true;
}

inline void widget::prefer_compact()
{
	extended_available = true;
	extended_preferred = false;
}

inline void widget::set_maximum_width(float width)
{
	maximum_width = width;
}

inline void widget::clear_maximum_width()
{
	maximum_width.reset();
}

inline auto widget::get_last_size() const -> glm::vec2
{
	return last_size;
}

inline auto widget::get_data_name() const -> std::string const&
{
	return data_name;
}

inline auto widget::get_available_width() const -> float
{
	if(maximum_width)
		return maximum_width.value();
	return ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().WindowPadding.x -
		   ImGui::GetStyle().FramePadding.x;
}

inline auto widget::is_extended_preferred() const -> bool
{
	extended_available = true;
	return extended_preferred;
}

} // namespace clk::gui