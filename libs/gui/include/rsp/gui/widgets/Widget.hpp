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
	std::string const resourceName;
	std::string const title;
	std::string const label;
	std::optional<float> maximumWidth;

public:
	Widget() = delete;
	Widget::Widget(std::string resourceName)
		: resourceName(resourceName), title(resourceName + ":"), label("##" + resourceName)
	{
	}
	virtual ~Widget() = default;

protected:
	virtual void drawContents() const = 0;

	float getLineWidth() const
	{
		return ImGui::GetContentRegionMax().x - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().WindowPadding.x -
			   ImGui::GetStyle().FramePadding.x;
	}

	bool isExtendedPreferred() const
	{
		extendedAvailable = true;
		return extendedPreferred;
	}

public:
	void Widget::draw() const
	{
		ImGui::PushID(this);
		ImGui::BeginGroup();
		ImGui::Text(title.c_str());
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

	float getAvailableWidth() const
	{
		if(maximumWidth)
			return maximumWidth.value();
		else
			return getLineWidth();
	}
};
} // namespace rsp::gui