#pragma once

#include "rsp/gui/widgets/Widget.hpp"
#include "rsp/util/Bounded.hpp"
#include "rsp/util/ColorRGB.hpp"
#include "rsp/util/ColorRGBA.hpp"
#include "rsp/util/TimeUnit.hpp"
#include "rsp/util/TypeList.hpp"

#include <imgui.h>

namespace rsp::gui
{
template <typename T>
class Viewer final : public Widget
{
public:
	Viewer() = delete;
	explicit Viewer(T const& resource, const std::string& resourceName);
	Viewer(Viewer const&) = delete;
	Viewer(Viewer&&) = delete;
	auto operator=(Viewer const&) -> Viewer& = delete;
	auto operator=(Viewer&&) -> Viewer& = delete;
	~Viewer() final = default;

	void drawContents() const override;

private:
	T const& resource;
};

using SupportedViewerTypes =
	meta::TypeList<bool, int, float, glm::vec2, glm::vec3, glm::vec4, Bounded<int>, Bounded<float>, Bounded<glm::vec2>,
		Bounded<glm::vec3>, Bounded<glm::vec4>, ColorRGB, ColorRGBA, std::chrono::nanoseconds>;

template <typename T>
inline auto makeViewer(T const& resource, std::string resourceName) -> std::unique_ptr<Widget>
{
	return std::make_unique<Viewer<T>>(resource, std::move(resourceName));
}

template <typename T>
Viewer<T>::Viewer(T const& resource, const std::string& resourceName) : Widget(resourceName), resource(resource)
{
}

template <typename T>
void Viewer<T>::drawContents() const
{
	// static_assert(false, "Not Implemented");
}

template <>
inline void Viewer<bool>::drawContents() const
{
	ImGui::SameLine();
	if(resource)
		ImGui::Text("On");
	else
		ImGui::Text("Off");
}

template <>
inline void Viewer<int>::drawContents() const
{
	ImGui::SameLine();
	ImGui::Text("%i", resource);
}

template <>
inline void Viewer<float>::drawContents() const
{
	ImGui::SameLine();
	ImGui::Text("%.3f", resource);
}

template <>
inline void Viewer<glm::vec2>::drawContents() const
{
	ImGui::Text("%.3f, %.3f", resource.x, resource.y);
}

template <>
inline void Viewer<glm::vec3>::drawContents() const
{
	ImGui::Text("%.3f, %.3f, %.3f", resource.x, resource.y, resource.z);
}

template <>
inline void Viewer<glm::vec4>::drawContents() const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", resource.x, resource.y, resource.z, resource.w);
}

template <>
inline void Viewer<Bounded<int>>::drawContents() const
{
	ImGui::Text("%i (%i - %i)", resource.getVal(), resource.getMin(), resource.getMax());
}

template <>
inline void Viewer<Bounded<float>>::drawContents() const
{
	ImGui::Text("%.3f (%.3f - %.3f)", resource.getVal(), resource.getMin(), resource.getMax());
}

template <>
inline void Viewer<Bounded<glm::vec2>>::drawContents() const
{
	ImGui::Text("%.3f, %.3f", resource.getVal().x, resource.getVal().y);
	if(isExtendedPreferred())
	{
		ImGui::Text("X (%.3f - %.3f)", resource.getMin()[0], resource.getMax()[0]);
		ImGui::Text("Y (%.3f - %.3f)", resource.getMin()[1], resource.getMax()[1]);
	}
}

template <>
inline void Viewer<Bounded<glm::vec3>>::drawContents() const
{
	ImGui::Text("%.3f, %.3f, %.3f", resource.getVal().x, resource.getVal().y, resource.getVal().z);
	if(isExtendedPreferred())
	{
		ImGui::Text("X (%.3f - %.3f)", resource.getMin()[0], resource.getMax()[0]);
		ImGui::Text("Y (%.3f - %.3f)", resource.getMin()[1], resource.getMax()[1]);
		ImGui::Text("Z (%.3f - %.3f)", resource.getMin()[2], resource.getMax()[2]);
	}
}

template <>
inline void Viewer<Bounded<glm::vec4>>::drawContents() const
{
	ImGui::Text(
		"%.3f, %.3f, %.3f, %.3f", resource.getVal().x, resource.getVal().y, resource.getVal().z, resource.getVal().w);
	if(isExtendedPreferred())
	{
		ImGui::Text("X (%.3f - %.3f)", resource.getMin()[0], resource.getMax()[0]);
		ImGui::Text("Y (%.3f - %.3f)", resource.getMin()[1], resource.getMax()[1]);
		ImGui::Text("Z (%.3f - %.3f)", resource.getMin()[2], resource.getMax()[2]);
		ImGui::Text("W (%.3f - %.3f)", resource.getMin()[3], resource.getMax()[3]);
	}
}

template <>
inline void Viewer<ColorRGB>::drawContents() const
{
	ImGui::Text("%.3f, %.3f, %.3f", resource.r(), resource.g(), resource.b());
	if(isExtendedPreferred())
	{
		auto s = getAvailableWidth();

		ImGui::ColorButton(label.c_str(), ImVec4(resource.r(), resource.g(), resource.b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton(label.c_str(), ImVec4(resource.r(), resource.g(), resource.b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void Viewer<ColorRGBA>::drawContents() const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", resource.r(), resource.g(), resource.b(), resource.a());
	if(isExtendedPreferred())
	{
		auto s = getAvailableWidth();

		ImGui::ColorButton(
			label.c_str(), resource, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton(label.c_str(), resource, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void Viewer<std::chrono::nanoseconds>::drawContents() const
{
	auto timeUnits = TimeUnit::decompose(resource);
	if(isExtendedPreferred())
	{
		for(auto const& unit : timeUnits)
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
		const int maxUnitsToDraw = 2;
		for(int i = 0; i < timeUnits.size(); i++)
		{
			if(timeUnits[i].value)
			{
				for(int j = i; j < timeUnits.size() && j - i < maxUnitsToDraw; j++)
				{
					ImGui::Text(("%i" + timeUnits[j].suffix).c_str(), timeUnits[j].value);
					ImGui::SameLine();
				}
				break;
			}
		}
		ImGui::NewLine();
	}
}

} // namespace rsp::gui