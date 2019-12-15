#pragma once

#include "rsp/base/port/Port.hpp"

namespace rsp
{
class Timestamp;

class DataPort : public virtual Port
{
public:
	struct ObserverFlags
	{
		enum
		{
			onFailedUpdate = 16,
			onDataRequested
		};
	};

protected:
	std::size_t dataTypeHash;

public:
	DataPort() = default;
	DataPort(DataPort const&) = delete;
	DataPort(DataPort&&) = default;
	DataPort& operator=(DataPort const& that) = delete;
	DataPort& operator=(DataPort&&) = default;
	virtual ~DataPort() = default;

public:
	virtual Timestamp const& getTimestamp() const = 0;
	[[nodiscard]] virtual bool update() const = 0;
	std::size_t getDataTypeHash() const
	{
		return dataTypeHash;
	}
};

} // namespace rsp