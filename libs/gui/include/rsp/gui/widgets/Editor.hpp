#pragma once

#include "rsp/base/Bounded.hpp"
#include "rsp/base/ColorRGB.hpp"
#include "rsp/base/ColorRGBA.hpp"
#include "rsp/base/TimeUnit.hpp"
#include "rsp/gui/widgets/Widget.hpp"

#include <imgui.h>

namespace rsp::gui
{
template <typename T>
class Editor final : public Widget
{
private:
	T& resource;

public:
	explicit Editor(T& resource, std::string resourceName) : Widget(resourceName), resource(resource)
	{
	}
	~Editor() = default;

public:
	void drawContents() const override
	{
		static_assert(false, "Not Implemented");
	}
};

template <typename T>
inline std::unique_ptr<Editor<T>> makeEditor(T& resource, std::string resourceName)
{
	return std::make_unique<Editor<T>>(resource, std::move(resourceName));
}

template <>
inline void Editor<bool>::drawContents() const
{
	ImGui::SameLine();
	ImGui::Checkbox(label.c_str(), &resource);
}

template <>
inline void Editor<int>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	ImGui::DragInt(label.c_str(), &resource);
}

template <>
inline void Editor<float>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	ImGui::DragFloat(label.c_str(), &resource);
}

template <>
inline void Editor<glm::vec2>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			ImGui::DragFloat(label.c_str(), &resource[i]);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			ImGui::DragFloat(label.c_str(), &resource[i]);
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void Editor<glm::vec3>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			ImGui::DragFloat(label.c_str(), &resource[i]);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			ImGui::DragFloat(label.c_str(), &resource[i]);
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void Editor<glm::vec4>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			ImGui::DragFloat(label.c_str(), &resource[i]);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			ImGui::DragFloat(label.c_str(), &resource[i]);
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void Editor<Bounded<int>>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	ImGui::SliderInt(label.c_str(), resource.data(), resource.getMin(), resource.getMax());
}

template <>
inline void Editor<Bounded<float>>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	ImGui::SliderFloat(label.c_str(), resource.data(), resource.getMin(), resource.getMax());
}

template <>
inline void Editor<Bounded<glm::vec2>>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			ImGui::SliderFloat(
				label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			ImGui::SliderFloat(
				label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]);
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void Editor<Bounded<glm::vec3>>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			ImGui::SliderFloat(
				label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			ImGui::SliderFloat(
				label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]);
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void Editor<Bounded<glm::vec4>>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			ImGui::SliderFloat(
				label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			ImGui::SliderFloat(
				label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]);
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void Editor<ColorRGB>::drawContents() const
{
	ImGui::PushItemWidth(getAvailableWidth());

	if(isExtendedPreferred())
	{
		ImGui::PushID(0);
		ImGui::ColorEdit3(label.c_str(), resource.data(),
			ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float);
		ImGui::PopID();

		ImGui::PushID(1);
		ImGui::ColorEdit3(label.c_str(), resource.data(),
			ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float);
		ImGui::PopID();

		ImGui::PushID(2);
		ImGui::ColorEdit3(label.c_str(), resource.data(),
			ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_Float);
		ImGui::PopID();

		ImGui::PushID(3);
		ImGui::ColorPicker3(label.c_str(), resource.data(),
			ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				ImGuiColorEditFlags_Float);
		ImGui::PopID();
	}
	else
	{
		ImGui::ColorEdit3(label.c_str(), resource.data(), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip);
	}
	ImGui::PopItemWidth();
}

template <>
inline void Editor<ColorRGBA>::drawContents() const
{
	ImGui::PushItemWidth(getAvailableWidth());
	if(isExtendedPreferred())
	{
		ImGui::PushID(0);
		ImGui::ColorEdit4(label.c_str(), resource.data(),
			ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float);
		ImGui::PopID();

		ImGui::PushID(1);
		ImGui::ColorEdit4(label.c_str(), resource.data(),
			ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				ImGuiColorEditFlags_NoAlpha);
		ImGui::PopID();

		ImGui::PushID(2);
		ImGui::ColorEdit4(label.c_str(), resource.data(),
			ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				ImGuiColorEditFlags_Float);
		ImGui::PopID();

		ImGui::PushID(3);
		ImGui::ColorPicker4(label.c_str(), resource.data(),
			ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				ImGuiColorEditFlags_Float);
		ImGui::PopID();
	}
	else
	{
		ImGui::ColorEdit4(label.c_str(), resource.data(), ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip);
	}
	ImGui::PopItemWidth();
}

template <>
inline void Editor<std::chrono::nanoseconds>::drawContents() const
{
	auto timeUnits = TimeUnit::decompose(resource);
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth() / 2.0f);
		int ct = 0;
		for(auto& unit : timeUnits)
		{
			int v = unit.value;
			ImGui::PushID(ct);
			ImGui::DragInt(label.c_str(), &v, 0.1f, 0, 0, ("%i " + unit.suffix).c_str());
			ImGui::PopID();
			unit.value = v;
			if(++ct % 2 != 0)
				ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	else
	{
		const int maxUnitsToDraw = 2;
		ImGui::PushItemWidth(getAvailableWidth() / 2.0f);
		for(int i = 0; i < timeUnits.size(); i++)
		{
			if(timeUnits[i].value || timeUnits.size() - i == maxUnitsToDraw)
			{
				for(int j = i; j < timeUnits.size() && j - i < maxUnitsToDraw; j++)
				{
					int v = timeUnits[j].value;
					ImGui::PushID(j);
					ImGui::DragInt(label.c_str(), &v, 0.1f, 0, 0, ("%i " + timeUnits[j].suffix).c_str());
					ImGui::PopID();
					timeUnits[j].value = v;
					ImGui::SameLine();
				}
				break;
			}
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	resource = TimeUnit::compose(timeUnits);
}

} // namespace rsp::gui