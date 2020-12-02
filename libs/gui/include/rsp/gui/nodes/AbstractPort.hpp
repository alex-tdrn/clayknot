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
public:
	AbstractPort() = default;
	explicit AbstractPort(rsp::Port* port);
	AbstractPort(AbstractPort&&) = default;
	AbstractPort(AbstractPort const&) = delete;
	auto operator=(AbstractPort&&) -> AbstractPort& = default;
	auto operator=(AbstractPort const&) -> AbstractPort& = delete;
	virtual ~AbstractPort();

	static auto getPinForID(ax::NodeEditor::PinId id) -> AbstractPort*;
	static auto getIDForPort(rsp::Port const* port) -> ax::NodeEditor::PinId;
	static auto getPinForPort(rsp::Port const* port) -> AbstractPort*;
	auto getID() const -> ax::NodeEditor::PinId;
	auto getPort() const -> rsp::Port*;
	void draw();
	auto getSize() const -> glm::vec2;
	auto isWidgetVisible() const -> bool;
	void setWidgetVisibility(bool visibility);
	void showWidget();
	void hideWidget();
	void toggleWidget();
	virtual auto canConnect(AbstractPort const* otherPin) const -> bool = 0;
	virtual void connect(AbstractPort* otherPin) = 0;

protected:
	ax::NodeEditor::PinId id = -1;
	rsp::Port* port = nullptr;
	Animation<float> anchorOffset;
	std::unique_ptr<Widget> widget;
	bool widgetVisible = true;
	glm::vec2 widgetSize{0.0f, 0.0f};

	static void placeAnchor(float height);
	void drawWidget();
	auto getWidgetSize() const -> glm::vec2;
	virtual auto calculateAnchorPosition() const -> ImVec2 = 0;
	virtual void drawContents() = 0;

private:
	static inline std::unordered_map<unsigned long long, AbstractPort*> pinMap;
	static inline std::unordered_map<rsp::Port const*, ax::NodeEditor::PinId> portMap;
	glm::vec2 size{0.0f, 0.0f};
};

inline AbstractPort::AbstractPort(rsp::Port* port) : id(uniqueID()), port(port)
{
	pinMap[id.Get()] = this;
	portMap[port] = id;
}

inline AbstractPort::~AbstractPort()
{
	pinMap.erase(id.Get());
	portMap.erase(port);
}

inline auto AbstractPort::getPinForID(ax::NodeEditor::PinId id) -> AbstractPort*
{
	if(pinMap.find(id.Get()) != pinMap.end())
		return pinMap[id.Get()];
	return nullptr;
}
inline auto AbstractPort::getIDForPort(rsp::Port const* port) -> ax::NodeEditor::PinId
{
	if(portMap.find(port) != portMap.end())
		return portMap[port];
	return {};
}
inline auto AbstractPort::getPinForPort(rsp::Port const* port) -> AbstractPort*
{
	return getPinForID(getIDForPort(port));
}

inline auto AbstractPort::getID() const -> ax::NodeEditor::PinId
{
	return id;
}

inline auto AbstractPort::getPort() const -> rsp::Port*
{
	return port;
}

inline void AbstractPort::draw()
{
	ImGui::BeginGroup();
	drawContents();
	ImGui::EndGroup();
	size = ImGui::GetItemRectSize();
}

inline auto AbstractPort::getSize() const -> glm::vec2
{
	return size;
}

inline auto AbstractPort::isWidgetVisible() const -> bool
{
	return widgetVisible;
}

inline void AbstractPort::setWidgetVisibility(bool visibility)
{
	widgetVisible = visibility;
}

inline void AbstractPort::showWidget()
{
	widgetVisible = true;
}
inline void AbstractPort::hideWidget()
{
	widgetVisible = false;
}
inline void AbstractPort::toggleWidget()
{
	widgetVisible = !widgetVisible;
}

inline void AbstractPort::placeAnchor(float height)
{
	ImGui::Dummy({10, height});
}

inline void AbstractPort::drawWidget()
{
	if(widgetVisible && widget)
		widget->draw();
	else
		ImGui::Text("%s", port->getName().c_str());
	widgetSize = ImGui::GetItemRectSize();

	if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(1))
		toggleWidget();
}

inline auto AbstractPort::getWidgetSize() const -> glm::vec2
{
	return widgetSize;
}

} // namespace rsp::gui