#pragma once

#include "rsp/base/Timestamp.hpp"
#include "rsp/base/node/AbstractNode.hpp"
#include "rsp/base/node/EventNode.hpp"
#include "rsp/base/port/InputEventPort.h"

#include <vector>

namespace rsp
{
class DataPort;

class AbstractSource : public virtual AbstractNode
{
public:
	struct InputEvents
	{
		enum
		{
			QueueRun = AbstractNode::InputEvents::SourceEvents
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
	bool isRunQueued() const
	{
		return timestamp.isReset();
	}

	void registerInputEvents() override
	{
		AbstractNode::registerInputEvents();
		registerInputEvent(InputEvents::QueueRun, "Queue Run", [this]() { queueRun(); });
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

	void queueRun()
	{
		timestamp.reset();
	}

	Timestamp const& getTimestamp() const
	{
		return timestamp;
	}
};

} // namespace rsp