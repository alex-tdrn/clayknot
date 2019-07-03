#pragma once

#include "rendershop/base/AbstractPort.hpp"

class Timestamp;

class AbstractDataPort : public virtual AbstractPort
{
protected:
	std::size_t dataTypeHash;
	mutable int requestCount = 0;

public:
	AbstractDataPort() = default;
	AbstractDataPort(AbstractDataPort const&) = delete;
	AbstractDataPort(AbstractDataPort&&) = default;
	AbstractDataPort& operator=(AbstractDataPort const& that) = delete;
	AbstractDataPort& operator=(AbstractDataPort&&) = default;
	virtual ~AbstractDataPort() = default;

public:
	virtual Timestamp const& getTimestamp() const = 0;
	virtual void update() const = 0;
	std::size_t getDataTypeHash() const
	{
		return dataTypeHash;
	}

	void requestFailed() const
	{
		requestCount++;
	}

	int getRequestCount() const
	{
		return requestCount;
	}
};
