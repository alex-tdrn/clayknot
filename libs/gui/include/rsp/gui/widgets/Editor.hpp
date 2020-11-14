#pragma once

#include "rsp/gui/widgets/Widget.hpp"
#include "rsp/util/Bounded.hpp"
#include "rsp/util/ColorRGB.hpp"
#include "rsp/util/ColorRGBA.hpp"
#include "rsp/util/TimeUnit.hpp"
#include "rsp/util/TypeList.hpp"

#include <functional>
#include <imgui.h>
#include <optional>

namespace rsp::gui
{
namespace impl
{
struct OwningTag
{
};
} // namespace impl

template <typename T>
class Editor final : public Widget
{
private:
	T& resource;
	std::optional<T> ownedResource;
	std::optional<std::function<void(T const&)>> modifiedCallback;

public:
	explicit Editor(T& resource, std::string resourceName) : Widget(resourceName), resource(resource)
	{
	}
	explicit Editor(impl::OwningTag tag, T ownedResource, std::string resourceName)
		: Widget(resourceName), ownedResource(ownedResource), resource(this->ownedResource.value())
	{
	}
	~Editor() = default;

protected:
	void resourceModified() const
	{
		if(modifiedCallback)
			(*modifiedCallback)(resource);
	}

public:
	void setModifiedCallback(std::optional<std::function<void(T const&)>> callback)
	{
		modifiedCallback = callback;
	}

	void drawContents() const override
	{
		// static_assert(false, "Not Implemented");
	}
};

using SupportedEditorTypes =
	meta::TypeList<bool, int, float, glm::vec2, glm::vec3, glm::vec4, Bounded<int>, Bounded<float>, Bounded<glm::vec2>,
		Bounded<glm::vec3>, Bounded<glm::vec4>, ColorRGB, ColorRGBA, std::chrono::nanoseconds>;

template <typename T>
inline std::unique_ptr<Widget> makeEditor(T& resource, std::string resourceName)
{
	return std::make_unique<Editor<T>>(resource, std::move(resourceName));
}

template <typename T, typename F>
inline std::unique_ptr<Widget> makeEditor(T& resource, std::string resourceName, F&& modifiedCallback)
{
	auto editor = std::make_unique<Editor<T>>(resource, std::move(resourceName));
	editor->setModifiedCallback(std::forward<F>(modifiedCallback));
	return editor;
}

template <typename T, typename F>
inline std::unique_ptr<Widget> makeOwningEditor(T initialResourceValue, std::string resourceName, F&& modifiedCallback)
{
	auto editor = std::make_unique<Editor<T>>(impl::OwningTag{}, initialResourceValue, std::move(resourceName));
	editor->setModifiedCallback(std::forward<F>(modifiedCallback));
	return editor;
}

template <>
inline void Editor<bool>::drawContents() const
{
	ImGui::SameLine();
	if(ImGui::Checkbox(label.c_str(), &resource))
		resourceModified();
}

template <>
inline void Editor<int>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	if(ImGui::DragInt(label.c_str(), &resource))
		resourceModified();
}

template <>
inline void Editor<float>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	if(ImGui::DragFloat(label.c_str(), &resource))
		resourceModified();
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
			if(ImGui::DragFloat(label.c_str(), &resource[i]))
				resourceModified();
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
			if(ImGui::DragFloat(label.c_str(), &resource[i]))
				resourceModified();
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
			if(ImGui::DragFloat(label.c_str(), &resource[i]))
				resourceModified();
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
			if(ImGui::DragFloat(label.c_str(), &resource[i]))
				resourceModified();
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
			if(ImGui::DragFloat(label.c_str(), &resource[i]))
				resourceModified();
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
			if(ImGui::DragFloat(label.c_str(), &resource[i]))
				resourceModified();
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
	if(ImGui::SliderInt(label.c_str(), resource.data(), resource.getMin(), resource.getMax()))
		resourceModified();
}

template <>
inline void Editor<Bounded<float>>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	if(ImGui::SliderFloat(label.c_str(), resource.data(), resource.getMin(), resource.getMax()))
		resourceModified();
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
			if(ImGui::SliderFloat(
				   label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]))
				resourceModified();
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
			if(ImGui::SliderFloat(
				   label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]))
				resourceModified();
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
			if(ImGui::SliderFloat(
				   label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]))
				resourceModified();
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
			if(ImGui::SliderFloat(
				   label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]))
				resourceModified();
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
			if(ImGui::SliderFloat(
				   label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]))
				resourceModified();
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
			if(ImGui::SliderFloat(
				   label.c_str(), &(resource.data()->operator[](i)), resource.getMin()[i], resource.getMax()[i]))
				resourceModified();
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
		if(ImGui::ColorEdit3(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit3(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit3(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker3(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit3(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
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
		if(ImGui::ColorEdit4(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit4(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit4(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker4(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit4(label.c_str(), resource.data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			resourceModified();
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
			if(ImGui::DragInt(label.c_str(), &v, 0.1f, 0, 0, ("%i " + unit.suffix).c_str()))
				resourceModified();
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
					if(ImGui::DragInt(label.c_str(), &v, 0.1f, 0, 0, ("%i " + timeUnits[j].suffix).c_str()))
						resourceModified();
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