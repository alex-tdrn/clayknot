#pragma once

#include "rsp/base/Port.hpp"
#include "rsp/gui/Animation.hpp"
#include "rsp/gui/UniqueID.hpp"
#include "rsp/gui/widgets/Widget.hpp"

#include <glm/glm.hpp>
#include <imgui_node_editor.h>
#include <unordered_map>

namespace rsp::gui
{
class AbstractPort
{
private:
	static inline std::unordered_map<unsigned long long, AbstractPort*> pinMap;
	static inline std::unordered_map<rsp::Port const*, ax::NodeEditor::PinId> portMap;
	glm::vec2 size{0.0f, 0.0f};

protected:
	ax::NodeEditor::PinId id = -1;
	rsp::Port* port = nullptr;
	Animation<float> anchorOffset;
	std::unique_ptr<Widget> widget;
	bool widgetVisible = true;
	glm::vec2 widgetSize{0.0f, 0.0f};

public:
	AbstractPort() = default;
	explicit AbstractPort(rsp::Port* port) : id(uniqueID()), port(port)
	{
		pinMap[id.Get()] = this;
		portMap[port] = id;
	}
	AbstractPort(AbstractPort&&) = default;
	AbstractPort(AbstractPort const&) = delete;
	auto operator=(AbstractPort&&) -> AbstractPort& = default;
	auto operator=(AbstractPort const&) -> AbstractPort& = delete;
	virtual ~AbstractPort()
	{
		pinMap.erase(id.Get());
		portMap.erase(port);
	}

protected:
	virtual auto calculateAnchorPosition() const -> ImVec2 = 0;
	virtual void drawContents() = 0;

	static void placeAnchor(float height)
	{
		ImGui::Dummy({10, height});
	}

	void drawWidget()
	{
		if(widgetVisible && widget)
			widget->draw();
		else
			ImGui::Text("%s", port->getName().c_str());
		widgetSize = ImGui::GetItemRectSize();

		if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(1))
			toggleWidget();
	}

	auto getWidgetSize() const -> glm::vec2
	{
		return widgetSize;
	}

public:
	static auto getPinForID(ax::NodeEditor::PinId id) -> AbstractPort*
	{
		if(pinMap.find(id.Get()) != pinMap.end())
			return pinMap[id.Get()];
		return nullptr;
	}
	static auto getIDForPort(rsp::Port const* port) -> ax::NodeEditor::PinId
	{
		if(portMap.find(port) != portMap.end())
			return portMap[port];
		return {};
	}
	static auto getPinForPort(rsp::Port const* port) -> AbstractPort*
	{
		return getPinForID(getIDForPort(port));
	}

	auto getID() const -> ax::NodeEditor::PinId
	{
		return id;
	}

	auto getPort() const -> rsp::Port*
	{
		return port;
	}

	void draw()
	{
		ImGui::BeginGroup();
		drawContents();
		ImGui::EndGroup();
		size = ImGui::GetItemRectSize();
	}

	auto getSize() const -> glm::vec2
	{
		return size;
	}

	auto isWidgetVisible() const -> bool
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

	virtual auto canConnect(AbstractPort const* otherPin) const -> bool = 0;
	virtual void connect(AbstractPort* otherPin) = 0;
};

} // namespace rsp::gui