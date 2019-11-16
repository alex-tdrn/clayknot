#pragma once

#include <string>
#include <set>

class AbstractPort
{
private:
	std::string name = "Unnamed";

public:
	AbstractPort() = default;
	AbstractPort(AbstractPort const&) = delete;
	AbstractPort(AbstractPort&&) = default;
	AbstractPort& operator=(AbstractPort const& that) = delete;
	AbstractPort& operator=(AbstractPort&&) = default;
	virtual ~AbstractPort() = default;

public:	
	virtual bool canConnect(AbstractPort* port) const = 0;
	virtual bool isConnected() const = 0;
	virtual void connect(AbstractPort* port) = 0;
	virtual void disconnect() = 0;
	virtual std::set<AbstractPort*> getConnections() const = 0;

	std::string const& getName() const
	{
		return name;
	}

	void setName(std::string const& name)
	{
		this->name = name;
	}

};
