#pragma once

#include "rshp/base/port/Port.hpp"

namespace rshp::base
{
		
	template<typename InputPort>
	class OutputPort : public virtual Port
	{
	protected:
		std::set<InputPort*> connections;

	public:
		OutputPort() = default;
		OutputPort(OutputPort const&) = delete;
		OutputPort(OutputPort&&) = default;
		OutputPort& operator=(OutputPort const& that) = delete;
		OutputPort& operator=(OutputPort&&) = default;
		virtual ~OutputPort() = default;

	public:
		bool isConnected() const final override
		{
			return !connections.empty();
		}

		void connect(Port* port) final override
		{
			connect(dynamic_cast<InputPort*>(port));
		}

		bool canConnect(Port* port) const final override
		{
			if(!dynamic_cast<InputPort*>(port))
				return false;
			else
				return true;
		}

		void disconnect() final override
		{
			while(!connections.empty())
			{
				disconnect(*connections.begin());
			}
		}

		std::set<Port*> getConnections() const final override
		{
			std::set<Port*> ret;
			for(auto port : connections)
				ret.insert(port);
			return ret;
		}

		void connect(InputPort* port)
		{
			if(port == nullptr)
				throw "Invalid port";

			if(connections.find(port) != connections.end())
				return;
			connections.insert(port);
			port->connect(this);
		}

		void disconnect(Port* port)
		{
			auto castedPort = dynamic_cast<InputPort*>(port);
			if(!castedPort)
				return;
			disconnect(castedPort);
		}

		void disconnect(InputPort* port)
		{
			if(connections.find(port) == connections.end())
				return;
			connections.erase(port);
			port->disconnect();
		}

	};

}