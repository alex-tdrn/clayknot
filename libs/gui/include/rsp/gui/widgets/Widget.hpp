#pragma once

#include <imgui.h>
#include <memory>
#include <optional>
#include <string>

namespace rsp::gui
{
class Widget
{
public:
	Widget() = delete;
	explicit Widget(std::string resourceName);
	Widget(Widget const&) = delete;
	Widget(Widget&&) = delete;
	auto operator=(Widget const&) -> Widget& = delete;
	auto operator=(Widget&&) -> Widget& = delete;
	virtual ~Widget() = default;

	void draw() const;
	void preferExtended();
	void preferCompact();
	void setMaximumWidth(float width);
	void clearMaximumWidth();
	auto getAvailableWidth() const -> float;

protected:
	std::string const title;
	std::string const label;
	std::string const resourceName;
	std::optional<float> maximumWidth;

	static auto getLineWidth() -> float;
	auto isExtendedPreferred() const -> bool;
	virtual void drawContents() const = 0;

private:
	mutable bool extendedAvailable = false;
	mutable bool extendedPreferred = false;
};

inline Widget::Widget(std::string resourceName)
	: title(resourceName + ":"), label("##" + resourceName), resourceName(std::move(resourceName))
{
}

inline void Widget::draw() const
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

inline void Widget::preferExtended()
{
	extendedPreferred = true;
}

inline void Widget::preferCompact()
{
	extendedPreferred = false;
}

inline void Widget::setMaximumWidth(float width)
{
	maximumWidth = width;
}

inline void Widget::clearMaximumWidth()
{
	maximumWidth.reset();
}

inline auto Widget::getAvailableWidth() const -> float
{
	if(maximumWidth)
		return maximumWidth.value();
	return getLineWidth();
}

inline auto Widget::getLineWidth() -> float
{
	return ImGui::GetContentRegionMax().x - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().WindowPadding.x -
		   ImGui::GetStyle().FramePadding.x;
}

inline auto Widget::isExtendedPreferred() const -> bool
{
	extendedAvailable = true;
	return extendedPreferred;
}

} // namespace rsp::gui