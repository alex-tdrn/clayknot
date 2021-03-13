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
	explicit Widget(std::string dataName);
	Widget(Widget const&) = delete;
	Widget(Widget&&) = delete;
	auto operator=(Widget const&) -> Widget& = delete;
	auto operator=(Widget&&) -> Widget& = delete;
	virtual ~Widget() = default;

	virtual auto clone() const -> std::unique_ptr<Widget> = 0;
	void draw() const;
	void enableTitle();
	void disableTitle();
	void preferExtended();
	void preferCompact();
	void setMaximumWidth(float width);
	void clearMaximumWidth();
	auto getLastSize() const -> glm::vec2;
	auto getDataName() const -> std::string const&;

protected:
	auto getAvailableWidth() const -> float;
	auto isExtendedPreferred() const -> bool;
	virtual auto isDataAvailable() const noexcept -> bool = 0;
	virtual void drawContents() const = 0;

private:
	std::string const dataName;
	bool drawTitle = true;
	mutable std::optional<float> maximumWidth;
	mutable glm::vec2 lastSize = {0.0f, 0.0f};
	mutable bool extendedAvailable = false;
	mutable bool extendedPreferred = false;
};

inline Widget::Widget(std::string dataName) : dataName(std::move(dataName))
{
}

inline void Widget::draw() const
{
	ImGui::PushID(this);
	if(ImGui::BeginPopup(dataName.c_str()))
	{
		bool wasExtendedPreferred = extendedPreferred;
		std::optional<float> oldMaximumWidth = maximumWidth;

		extendedPreferred = true;
		maximumWidth = ImGui::GetIO().DisplaySize.y * 0.75;

		if(drawTitle)
			ImGui::Text("%s", dataName.c_str());
		if(isDataAvailable())
			drawContents();
		ImGui::EndPopup();

		extendedPreferred = wasExtendedPreferred;
		maximumWidth = oldMaximumWidth;
	}
	else
	{
		ImGui::BeginGroup();
		if(drawTitle)
			ImGui::Text("%s", dataName.c_str());
		if(extendedAvailable)
		{
			if(drawTitle)
				ImGui::SameLine();
			if(ImGui::SmallButton((extendedPreferred ? "-" : "+")))
				extendedPreferred = !extendedPreferred;
			if(extendedPreferred)
			{
				ImGui::SameLine();
				if(ImGui::SmallButton("+"))
					ImGui::OpenPopup(dataName.c_str());
			}
		}
		if(isDataAvailable())
			drawContents();
		else
			ImGui::Text("No data...");
		ImGui::EndGroup();
		lastSize = ImGui::GetItemRectSize();
	}
	ImGui::PopID();
}

inline void Widget::enableTitle()
{
	drawTitle = true;
}

inline void Widget::disableTitle()
{
	drawTitle = false;
}

inline void Widget::preferExtended()
{
	extendedAvailable = true;
	extendedPreferred = true;
}

inline void Widget::preferCompact()
{
	extendedAvailable = true;
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

inline auto Widget::getLastSize() const -> glm::vec2
{
	return lastSize;
}

inline auto Widget::getDataName() const -> std::string const&
{
	return dataName;
}

inline auto Widget::getAvailableWidth() const -> float
{
	if(maximumWidth)
		return maximumWidth.value();
	return ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().WindowPadding.x -
		   ImGui::GetStyle().FramePadding.x;
}

inline auto Widget::isExtendedPreferred() const -> bool
{
	extendedAvailable = true;
	return extendedPreferred;
}

} // namespace rsp::gui