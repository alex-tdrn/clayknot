#pragma once

#include "rsp/base/port/Port.hpp"

namespace rsp
{
class Timestamp;

class DataPort : public virtual Port
{
protected:
	std::size_t dataTypeHash;
	mutable int requestCount = 0;

public:
	DataPort() = default;
	DataPort(DataPort const&) = delete;
	DataPort(DataPort&&) = default;
	DataPort& operator=(DataPort const& that) = delete;
	DataPort& operator=(DataPort&&) = default;
	virtual ~DataPort() = default;

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

} // namespace rsp