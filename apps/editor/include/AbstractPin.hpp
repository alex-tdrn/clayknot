#pragma once
#include "UniqueID.hpp"
#include "rshp/base/AbstractPort.hpp"
#include "Animation.hpp"

#include <imgui_node_editor.h>
#include <unordered_map>

class AbstractPin
{
private:
	static inline std::unordered_map<unsigned long long, AbstractPin*> pinMap;
	static inline std::unordered_map<AbstractPort const*, ax::NodeEditor::PinId> portMap;

protected:
	ax::NodeEditor::PinId id = -1;
	AbstractPort* port = nullptr;
	Animation<float> anchorOffset; 

public:
	AbstractPin() = default;
	AbstractPin(AbstractPort * port)
		: id(uniqueID()), port(port)
	{
		pinMap[id.Get()] = this;
		portMap[port] = id;
	}
	AbstractPin(AbstractPin&&) = default;
	AbstractPin(AbstractPin const&) = delete;
	AbstractPin& operator=(AbstractPin&&) = default;
	AbstractPin& operator=(AbstractPin const&) = delete;
	virtual ~AbstractPin()
	{
		pinMap.erase(id.Get());
		portMap.erase(port);
	}

protected:
	virtual ImVec2 calculateAnchorPosition() const = 0;

public:
	static AbstractPin* getPinForID(ax::NodeEditor::PinId id)
	{
		if(pinMap.find(id.Get()) != pinMap.end())
			return pinMap[id.Get()];
		return nullptr;
	}
	static ax::NodeEditor::PinId getIDForPort(AbstractPort const* port)
	{
		if(portMap.find(port) != portMap.end())
			return portMap[port];
		return {};
	}
	static AbstractPin* getPinForPort(AbstractPort const* port)
	{
		return getPinForID(getIDForPort(port));
	}

	ax::NodeEditor::PinId getID() const
	{
		return id;
	}

	AbstractPort* getPort() const
	{
		return port;
	}

	virtual bool canConnect(AbstractPin* otherPin) = 0;
	virtual void connect(AbstractPin* otherPin) = 0;
	virtual void draw() = 0;
	virtual ImVec2 calculateSize() const = 0;
};