#pragma once

#include "rsp/base/port/Port.hpp"

namespace rsp
{
class EventPort : public virtual Port
{
public:
	struct ObserverFlags
	{
		enum
		{
			onTriggered = 32,
			onTriggerFailed
		};
	};

public:
	EventPort() = default;
	EventPort(EventPort const&) = delete;
	EventPort(EventPort&&) = default;
	EventPort& operator=(EventPort const& that) = delete;
	EventPort& operator=(EventPort&&) = default;
	virtual ~EventPort() = default;
};

} // namespace rsp