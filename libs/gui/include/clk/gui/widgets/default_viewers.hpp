#pragma once

#include "clk/gui/widgets/viewer.hpp"
#include "clk/util/bounded.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"
#include "clk/util/time_unit.hpp"

#include <range/v3/view.hpp>

namespace clk::gui
{

template <>
inline void viewer_of<bool>::draw_contents(bool const& data) const
{
	ImGui::SameLine();
	if(data) // TODO use icons later when integrating icon fonts
		ImGui::Text("True");
	else
		ImGui::Text("False");
}

template <>
inline void viewer_of<int>::draw_contents(int const& data) const
{
	ImGui::SameLine();
	ImGui::Text("%i", data);
}

template <>
inline void viewer_of<float>::draw_contents(float const& data) const
{
	ImGui::SameLine();
	ImGui::Text("%.3f", data);
}

template <>
inline void viewer_of<glm::vec2>::draw_contents(glm::vec2 const& data) const
{
	ImGui::Text("%.3f, %.3f", data.x, data.y);
}

template <>
inline void viewer_of<glm::vec3>::draw_contents(glm::vec3 const& data) const
{
	ImGui::Text("%.3f, %.3f, %.3f", data.x, data.y, data.z);
}

template <>
inline void viewer_of<glm::vec4>::draw_contents(glm::vec4 const& data) const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", data.x, data.y, data.z, data.w);
}

template <>
inline void viewer_of<clk::bounded<int>>::draw_contents(clk::bounded<int> const& data) const
{
	ImGui::Text("%i (%i - %i)", data.val(), data.min(), data.max());
}

template <>
inline void viewer_of<clk::bounded<float>>::draw_contents(clk::bounded<float> const& data) const
{
	ImGui::Text("%.3f (%.3f - %.3f)", data.val(), data.min(), data.max());
}

template <>
inline void viewer_of<clk::bounded<glm::vec2>>::draw_contents(clk::bounded<glm::vec2> const& data) const
{
	ImGui::Text("%.3f, %.3f", data.val().x, data.val().y);
	if(extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data.min()[0], data.max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data.min()[1], data.max()[1]);
	}
}

template <>
inline void viewer_of<clk::bounded<glm::vec3>>::draw_contents(clk::bounded<glm::vec3> const& data) const
{
	ImGui::Text("%.3f, %.3f, %.3f", data.val().x, data.val().y, data.val().z);
	if(extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data.min()[0], data.max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data.min()[1], data.max()[1]);
		ImGui::Text("Z (%.3f - %.3f)", data.min()[2], data.max()[2]);
	}
}

template <>
inline void viewer_of<clk::bounded<glm::vec4>>::draw_contents(clk::bounded<glm::vec4> const& data) const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", data.val().x, data.val().y, data.val().z, data.val().w);
	if(extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data.min()[0], data.max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data.min()[1], data.max()[1]);
		ImGui::Text("Z (%.3f - %.3f)", data.min()[2], data.max()[2]);
		ImGui::Text("W (%.3f - %.3f)", data.min()[3], data.max()[3]);
	}
}

template <>
inline void viewer_of<clk::color_rgb>::draw_contents(clk::color_rgb const& data) const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f", data.r(), data.g(), data.b());
	if(extended_preferred())
	{
		auto s = available_width();

		ImGui::ColorButton("##", ImVec4(data.r(), data.g(), data.b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", ImVec4(data.r(), data.g(), data.b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void viewer_of<clk::color_rgba>::draw_contents(clk::color_rgba const& data) const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f, A:%.3f", data.r(), data.g(), data.b(), data.a());
	if(extended_preferred())
	{
		auto s = available_width();

		ImGui::ColorButton("##", data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void viewer_of<std::chrono::nanoseconds>::draw_contents(std::chrono::nanoseconds const& data) const
{
	auto time_units = time_unit::decompose(data);
	if(extended_preferred())
	{
		for(auto const& unit : time_units)
		{
			if(unit.value != 0)
			{
				ImGui::Text(("%i" + unit.suffix).c_str(), unit.value);
				ImGui::SameLine();
			}
		}
	}
	else
	{
		const std::size_t max_units_to_draw = 2;
		auto non_empty_units = time_units | ranges::views::drop_while([](auto unit) {
			return unit.value <= 0;
		});

		for(auto const& unit : non_empty_units | ranges::views::take(max_units_to_draw))
		{
			ImGui::Text(("%i" + unit.suffix).c_str(), unit.value);
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}
}
} // namespace clk::gui