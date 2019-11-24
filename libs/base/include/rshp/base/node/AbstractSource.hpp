#pragma once

#include "rshp/base/Timestamp.hpp"
#include "rshp/base/node/AbstractNode.hpp"
#include "rshp/base/node/EventNode.hpp"
#include "rshp/base/port/InputEventPort.h"

#include <vector>

namespace rshp::base
{
class DataPort;

class AbstractSource : public virtual AbstractNode
{
public:
	struct InputEvents
	{
		enum
		{
			QueueUpdate = AbstractNode::InputEvents::SourceEvents
		};
	};

	struct OutputEvents
	{
		enum
		{
		};
	};

protected:
	std::vector<DataPort*> abstractOutputDataPorts;
	mutable Timestamp timestamp;

public:
	AbstractSource() = default;
	AbstractSource(AbstractSource const&) = delete;
	AbstractSource(AbstractSource&&) = default;
	AbstractSource& operator=(AbstractSource const&) = delete;
	AbstractSource& operator=(AbstractSource&&) = default;
	virtual ~AbstractSource() = default;

protected:
	bool isUpdateQueued() const
	{
		return timestamp.isReset();
	}

	void registerInputEvents() override
	{
		AbstractNode::registerInputEvents();
		registerInputEvent(InputEvents::QueueUpdate, "Queue Update", [this]() { queueUpdate(); });
	}

	void registerOutputEvents() override
	{
		AbstractNode::registerOutputEvents();
	}

public:
	std::vector<DataPort*> const& getOutputDataPorts() const
	{
		return abstractOutputDataPorts;
	}

	void queueUpdate()
	{
		timestamp.reset();
	}

	Timestamp const& getTimestamp() const
	{
		return timestamp;
	}
};

} // namespace rshp::base