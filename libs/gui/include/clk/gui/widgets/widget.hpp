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
	widget() = default;
	widget(widget const&) = delete;
	widget(widget&&) = delete;
	auto operator=(widget const&) -> widget& = delete;
	auto operator=(widget&&) -> widget& = delete;
	virtual ~widget() = default;

	virtual auto clone() const -> std::unique_ptr<widget> = 0;
	virtual void copy(widget const& other) = 0;

	void draw() const;
	void enable_title();
	void disable_title();
	void prefer_extended();
	void prefer_compact();
	void set_maximum_width(float width);
	void clear_maximum_width();
	auto last_size() const -> glm::vec2;
	void set_name(std::string_view name);
	auto name() const -> std::string_view;
	void set_interactivity(bool interactive) noexcept;
	auto is_interactive() const noexcept -> bool;

protected:
	auto available_width() const -> float;
	auto extended_preferred() const -> bool;
	virtual void draw_contents() const = 0;

private:
	std::string _name;
	bool _draw_title = true;
	bool _interactive = true;
	mutable std::optional<float> _maximum_width;
	mutable glm::vec2 _last_size = {0.0f, 0.0f};
	mutable bool _extended_available = false;
	mutable bool _extended_preferred = false;
};

inline void widget::copy(widget const& other)
{
	_name = other._name;
	_draw_title = other._draw_title;
	_interactive = other._interactive;
	_maximum_width = other._maximum_width;
	_last_size = other._last_size;
	_extended_available = other._extended_available;
	_extended_preferred = other._extended_preferred;
}

inline void widget::draw() const
{
	ImGui::PushID(this);
	if(ImGui::BeginPopup(_name.c_str()))
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

inline void widget::set_interactivity(bool interactive) noexcept
{
	_interactive = interactive;
}

inline auto widget::is_interactive() const noexcept -> bool
{
	return _interactive;
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

inline auto widget::last_size() const -> glm::vec2
{
	return _last_size;
}

inline void widget::set_name(std::string_view name)
{
	_name = name;
}

inline auto widget::name() const -> std::string_view
{
	return _name;
}

inline auto widget::available_width() const -> float
{
	if(_maximum_width)
		return _maximum_width.value();
	return ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().WindowPadding.x -
		   ImGui::GetStyle().FramePadding.x;
}

inline auto widget::extended_preferred() const -> bool
{
	_extended_available = true;
	return _extended_preferred;
}

} // namespace clk::gui