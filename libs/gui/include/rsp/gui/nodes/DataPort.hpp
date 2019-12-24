#pragma once

#include "rsp/base/port/DataPort.hpp"
#include "rsp/gui/widgets/Widget.hpp"

namespace rsp::gui
{
class DataPort
{
protected:
	rsp::DataPort* dataPort;
	std::unique_ptr<Widget> widget;
	bool widgetVisible = true;
	glm::vec2 widgetSize{0.0f, 0.0f};

public:
	DataPort() = default;
	DataPort(rsp::DataPort* port) : dataPort(port)
	{
	}
	DataPort(DataPort&&) = default;
	DataPort(DataPort const&) = delete;
	DataPort& operator=(DataPort&&) = default;
	DataPort& operator=(DataPort const&) = delete;
	~DataPort() = default;

protected:
	void drawWidget()
	{
		if(widgetVisible && widget)
			widget->draw();
		else
			ImGui::Text(dataPort->getName().c_str());
		widgetSize = ImGui::GetItemRectSize();

		if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(1))
			toggleWidget();
	}

	glm::vec2 getWidgetSize() const
	{
		return widgetSize;
	}

public:
	bool isWidgetVisible() const
	{
		return widgetVisible;
	}

	void setWidgetVisibility(bool visibility)
	{
		widgetVisible = visibility;
	}

	void showWidget()
	{
		widgetVisible = true;
	}
	void hideWidget()
	{
		widgetVisible = false;
	}
	void toggleWidget()
	{
		widgetVisible = !widgetVisible;
	}
};

} // namespace rsp::gui