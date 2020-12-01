#pragma once

#include <imgui.h>
#include <memory>
#include <optional>
#include <string>

namespace rsp::gui
{
class Widget
{
private:
	mutable bool extendedAvailable = false;
	mutable bool extendedPreferred = false;

protected:
	std::string const title;
	std::string const label;
	std::string const resourceName;
	std::optional<float> maximumWidth;

public:
	Widget() = delete;
	explicit Widget(std::string resourceName)
		: title(resourceName + ":"), label("##" + resourceName), resourceName(std::move(resourceName))
	{
	}
	Widget(Widget const&) = delete;
	Widget(Widget&&) = delete;
	auto operator=(Widget const&) -> Widget& = delete;
	auto operator=(Widget&&) -> Widget& = delete;
	virtual ~Widget() = default;

protected:
	virtual void drawContents() const = 0;

	static auto getLineWidth() -> float
	{
		return ImGui::GetContentRegionMax().x - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().WindowPadding.x -
			   ImGui::GetStyle().FramePadding.x;
	}

	auto isExtendedPreferred() const -> bool
	{
		extendedAvailable = true;
		return extendedPreferred;
	}

public:
	void draw() const
	{
		ImGui::PushID(this);
		ImGui::BeginGroup();
		ImGui::Text("%s", title.c_str());
		if(extendedAvailable)
		{
			ImGui::SameLine();
			if(ImGui::SmallButton(extendedPreferred ? "-" : "+"))
				extendedPreferred = !extendedPreferred;
		}

		drawContents();
		ImGui::EndGroup();
		ImGui::PopID();
	}

	void preferExtended()
	{
		extendedPreferred = true;
	}

	void preferCompact()
	{
		extendedPreferred = false;
	}

	void setMaximumWidth(float width)
	{
		maximumWidth = width;
	}

	void clearMaximumWidth()
	{
		maximumWidth.reset();
	}

	auto getAvailableWidth() const -> float
	{
		if(maximumWidth)
			return maximumWidth.value();
		return getLineWidth();
	}
};
} // namespace rsp::gui