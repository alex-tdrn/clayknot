#pragma once

#include "clk/gui/widgets/editor.hpp"
#include "clk/util/bounded.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"
#include "clk/util/time_unit.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <limits>
#include <range/v3/view.hpp>
#include <string>
#include <type_traits>

namespace clk::gui
{
template <>
inline auto editor_of<bool>::draw_contents(bool& data) const -> bool
{
	ImGui::SameLine();
	return ImGui::Checkbox("##", &data);
}

namespace impl
{
template <typename Scalar>
constexpr auto fundamental_type_to_scalar_enum() -> ImGuiDataType_
{
	constexpr auto size = sizeof(Scalar);
	if constexpr(std::is_floating_point_v<Scalar>)
	{
		if constexpr(size == 4)
		{
			return ImGuiDataType_Float;
		}
		else if constexpr(size == 8)
		{
			return ImGuiDataType_Double;
		}
	}
	else if constexpr(std::is_unsigned_v<Scalar>)
	{
		if constexpr(size == 1)
		{
			return ImGuiDataType_U8;
		}
		else if constexpr(size == 2)
		{
			return ImGuiDataType_U16;
		}
		else if constexpr(size == 4)
		{
			return ImGuiDataType_U32;
		}
		else if constexpr(size == 8)
		{
			return ImGuiDataType_U64;
		}
	}
	else
	{
		if constexpr(size == 1)
		{
			return ImGuiDataType_S8;
		}
		else if constexpr(size == 2)
		{
			return ImGuiDataType_S16;
		}
		else if constexpr(size == 4)
		{
			return ImGuiDataType_S32;
		}
		else if constexpr(size == 8)
		{
			return ImGuiDataType_S64;
		}
	}

	return ImGuiDataType_COUNT;
}

} // namespace impl

template <>
inline auto editor_of<signed char>::draw_contents(signed char& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<signed char>::min();
	auto max = std::numeric_limits<signed char>::max();
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<signed char>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<unsigned char>::draw_contents(unsigned char& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<unsigned char>::min();
	auto max = std::numeric_limits<unsigned char>::max();
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<unsigned char>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<short int>::draw_contents(short int& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<short int>::min();
	auto max = std::numeric_limits<short int>::max();
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<short int>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<unsigned short int>::draw_contents(unsigned short int& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<unsigned short int>::min();
	auto max = std::numeric_limits<unsigned short int>::max();
	return ImGui::DragScalar(
		"##", impl::fundamental_type_to_scalar_enum<unsigned short int>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<int>::draw_contents(int& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<int>::min();
	auto max = std::numeric_limits<int>::max();
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<int>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<unsigned int>::draw_contents(unsigned int& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<unsigned int>::min();
	auto max = std::numeric_limits<unsigned int>::max();
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<unsigned int>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<long int>::draw_contents(long int& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<long int>::min();
	auto max = std::numeric_limits<long int>::max();
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<long int>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<unsigned long int>::draw_contents(unsigned long int& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<unsigned long int>::min();
	auto max = std::numeric_limits<unsigned long int>::max();
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<unsigned long int>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<long long int>::draw_contents(long long int& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<long long int>::min();
	auto max = std::numeric_limits<long long int>::max();
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<long long int>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<unsigned long long int>::draw_contents(unsigned long long int& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	auto min = std::numeric_limits<unsigned long long int>::min();
	auto max = std::numeric_limits<unsigned long long int>::max();
	return ImGui::DragScalar(
		"##", impl::fundamental_type_to_scalar_enum<unsigned long long int>(), &data, 1.0f, &min, &max);
}

template <>
inline auto editor_of<float>::draw_contents(float& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<float>(), &data, 0.01f);
}

template <>
inline auto editor_of<double>::draw_contents(double& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	return ImGui::DragScalar("##", impl::fundamental_type_to_scalar_enum<double>(), &data, 0.01f);
}

template <>
inline auto editor_of<glm::vec2>::draw_contents(glm::vec2& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<glm::vec3>::draw_contents(glm::vec3& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<glm::vec4>::draw_contents(glm::vec4& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<clk::bounded<int>>::draw_contents(clk::bounded<int>& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	return ImGui::SliderInt("##", data.data(), data.min(), data.max());
}

template <>
inline auto editor_of<clk::bounded<float>>::draw_contents(clk::bounded<float>& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	return ImGui::SliderFloat("##", data.data(), data.min(), data.max());
}

template <>
inline auto editor_of<clk::bounded<glm::vec2>>::draw_contents(clk::bounded<glm::vec2>& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<clk::bounded<glm::vec3>>::draw_contents(clk::bounded<glm::vec3>& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<clk::bounded<glm::vec4>>::draw_contents(clk::bounded<glm::vec4>& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<clk::color_rgb>::draw_contents(clk::color_rgb& data) const -> bool
{
	auto data_modified = false;
	ImGui::PushItemWidth(available_width());

	if(extended_preferred())
	{
		ImGui::PushID(0);
		if(ImGui::ColorEdit3("##", data.data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit3("##", data.data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit3("##", data.data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker3("##", data.data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit3("##", data.data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
	}
	ImGui::PopItemWidth();
	return data_modified;
}

template <>
inline auto editor_of<clk::color_rgba>::draw_contents(clk::color_rgba& data) const -> bool
{
	auto data_modified = false;
	ImGui::PushItemWidth(available_width());
	if(extended_preferred())
	{
		ImGui::PushID(0);
		if(ImGui::ColorEdit4("##", data.data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit4("##", data.data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit4("##", data.data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker4("##", data.data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit4("##", data.data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
	}
	ImGui::PopItemWidth();
	return data_modified;
}

template <>
inline auto editor_of<std::chrono::nanoseconds>::draw_contents(std::chrono::nanoseconds& data) const -> bool
{
	auto data_modified = false;
	auto time_units = time_unit::decompose(data);
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width() / 2.0f);
		int ct = 0;
		for(auto& unit : time_units)
		{
			int v = unit.value;
			ImGui::PushID(ct);
			if(ImGui::DragInt("##", &v, 0.1f, 0, 0, ("%i " + unit.suffix).c_str()))
				data_modified = true;
			ImGui::PopID();
			unit.value = static_cast<short>(v);
			if(++ct % 2 != 0)
				ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 2.0f);
		const std::size_t max_units_to_draw = 2;

		auto non_empty_units = time_units | ranges::views::drop_while([&, dropped = std::size_t(0)](auto unit) mutable {
			dropped++;
			return unit.value <= 0 && time_units.size() - dropped < max_units_to_draw;
		});

		for(auto [index, unit] : ranges::views::enumerate(non_empty_units | ranges::views::take(max_units_to_draw)))
		{
			int v = unit.value;
			ImGui::PushID(static_cast<int>(index));
			if(ImGui::DragInt("##", &v, 0.1f, 0, 0, ("%i " + unit.suffix).c_str()))
				data_modified = true;
			ImGui::PopID();
			unit.value = static_cast<short>(v);
			ImGui::SameLine();
		}

		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	data = time_unit::compose(time_units);

	return data_modified;
}

template <>
inline auto editor_of<char>::draw_contents(char& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	std::array<char, 2> buffer = {data, '\0'};

	if(ImGui::InputText(
		   "##", buffer.data(), 2, ImGuiInputTextFlags_AlwaysOverwrite | ImGuiInputTextFlags_AutoSelectAll))
	{
		data = buffer[0];
		return true;
	}
	return false;
}

template <>
inline auto editor_of<std::string>::draw_contents(std::string& data) const -> bool
{
	ImGui::PushItemWidth(available_width());
	return ImGui::InputText("##", &data);
}
} // namespace clk::gui