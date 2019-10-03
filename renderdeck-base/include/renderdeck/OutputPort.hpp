#pragma once
#include "renderdeck/AbstractPort.hpp"

#include <set>

template<typename InputPort>
class OutputPort : public virtual AbstractPort
{
private:
	std::set<InputPort*> connections;

public:
	OutputPort() = default;
	OutputPort(OutputPort const&) = delete;
	OutputPort(OutputPort&&) = delete;
	OutputPort& operator=(OutputPort const& that) = delete;
	OutputPort& operator=(OutputPort&&) = delete;
	~OutputPort() = default;

public:
	void connect(InputPort* port)
	{
		if(connections.find(port) != connections.end())
			return;
		connections.insert(port);
		port->connect(this);
	}

	void connect(AbstractPort* port) final override
	{
		if(!canConnect(port))
			return;
		connect(static_cast<InputPort*>(port));
	}

	bool canConnect(AbstractPort* port) final override
	{
		if(!dynamic_cast<InputPort*>(port))
			return false;
		else
			return true;
	}

	void disconnect(InputPort* port)
	{
		if(connections.find(port) == connections.end())
			return;
		connections.erase(port);
		port->disconnect();
	}

	void disconnect() final override
	{
		while(!connections.empty())
		{
			disconnect(*connections.begin());
		}
	}

};
