#pragma once

#include "rsp/base/port/Port.hpp"
#include "rsp/gui/Animation.hpp"
#include "rsp/gui/UniqueID.hpp"

#include <imgui_node_editor.h>
#include <unordered_map>

namespace rsp::gui
{
class AbstractPort
{
private:
	static inline std::unordered_map<unsigned long long, AbstractPort*> pinMap;
	static inline std::unordered_map<rsp::Port const*, ax::NodeEditor::PinId> portMap;

protected:
	ax::NodeEditor::PinId id = -1;
	rsp::Port* port = nullptr;
	Animation<float> anchorOffset;

public:
	AbstractPort() = default;
	AbstractPort(rsp::Port* port) : id(uniqueID()), port(port)
	{
		pinMap[id.Get()] = this;
		portMap[port] = id;
	}
	AbstractPort(AbstractPort&&) = default;
	AbstractPort(AbstractPort const&) = delete;
	AbstractPort& operator=(AbstractPort&&) = default;
	AbstractPort& operator=(AbstractPort const&) = delete;
	virtual ~AbstractPort()
	{
		pinMap.erase(id.Get());
		portMap.erase(port);
	}

protected:
	virtual ImVec2 calculateAnchorPosition() const = 0;

public:
	static AbstractPort* getPinForID(ax::NodeEditor::PinId id)
	{
		if(pinMap.find(id.Get()) != pinMap.end())
			return pinMap[id.Get()];
		return nullptr;
	}
	static ax::NodeEditor::PinId getIDForPort(rsp::Port const* port)
	{
		if(portMap.find(port) != portMap.end())
			return portMap[port];
		return {};
	}
	static AbstractPort* getPinForPort(rsp::Port const* port)
	{
		return getPinForID(getIDForPort(port));
	}

	ax::NodeEditor::PinId getID() const
	{
		return id;
	}

	rsp::Port* getPort() const
	{
		return port;
	}

	virtual bool canConnect(AbstractPort* otherPin) = 0;
	virtual void connect(AbstractPort* otherPin) = 0;
	virtual void draw() = 0;
	virtual ImVec2 calculateSize() const = 0;
};

} // namespace rsp::gui