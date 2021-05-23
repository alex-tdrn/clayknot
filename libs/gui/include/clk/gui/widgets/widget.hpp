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
	std::string const _data_name;
	bool _draw_title = true;
	mutable std::optional<float> _maximum_width;
	mutable glm::vec2 _last_size = {0.0f, 0.0f};
	mutable bool _extended_available = false;
	mutable bool _extended_preferred = false;
};

inline widget::widget(std::string data_name) : _data_name(std::move(data_name))
{
}

inline void widget::draw() const
{
	ImGui::PushID(this);
	if(ImGui::BeginPopup(_data_name.c_str()))
	{
		bool was_extended_preferred = _extended_preferred;
		std::optional<float> old_maximum_width = _maximum_width;

		_extended_preferred = true;
		_maximum_width = ImGui::GetIO().DisplaySize.y * 0.75;

		if(_draw_title)
			ImGui::Text("%s", _data_name.c_str());
		if(is_data_available())
			draw_contents();
		ImGui::EndPopup();

		_extended_preferred = was_extended_preferred;
		_maximum_width = old_maximum_width;
	}
	else
	{
		ImGui::BeginGroup();
		if(_draw_title)
			ImGui::Text("%s", _data_name.c_str());
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
					ImGui::OpenPopup(_data_name.c_str());
			}
		}
		if(is_data_available())
			draw_contents();
		else
			ImGui::Text("No data...");
		ImGui::EndGroup();
		_last_size = ImGui::GetItemRectSize();
	}
	ImGui::PopID();
}

inline void widget::enable_title()
{
	_draw_title = true;
}

inline void widget::disable_title()
{
	_draw_title = false;
}

inline void widget::prefer_extended()
{
	_extended_available = true;
	_extended_preferred = true;
}

inline void widget::prefer_compact()
{
	_extended_available = true;
	_extended_preferred = false;
}

inline void widget::set_maximum_width(float width)
{
	_maximum_width = width;
}

inline void widget::clear_maximum_width()
{
	_maximum_width.reset();
}

inline auto widget::get_last_size() const -> glm::vec2
{
	return _last_size;
}

inline auto widget::get_data_name() const -> std::string const&
{
	return _data_name;
}

inline auto widget::get_available_width() const -> float
{
	if(_maximum_width)
		return _maximum_width.value();
	return ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().WindowPadding.x -
		   ImGui::GetStyle().FramePadding.x;
}

inline auto widget::is_extended_preferred() const -> bool
{
	_extended_available = true;
	return _extended_preferred;
}

} // namespace clk::gui