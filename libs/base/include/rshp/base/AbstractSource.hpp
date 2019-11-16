#pragma once

#include "rshp/base/AbstractPipe.hpp"
#include "rshp/base/Timestamp.hpp"
#include "rshp/base/EventPipe.hpp"
#include "rshp/base/InputEventPort.hpp"

#include <vector>

class AbstractDataPort;

class AbstractSource : public virtual AbstractPipe
{
public:
	struct InputEvents
	{
		enum
		{
			QueueUpdate = AbstractPipe::InputEvents::SourceEvents
		};
	};

	struct OutputEvents
	{
		enum
		{
		};
	};

protected:
	std::vector<AbstractDataPort*> abstractOutputDataPorts;
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
		AbstractPipe::registerInputEvents();
		registerInputEvent(InputEvents::QueueUpdate, "Queue Update", [this]() {
			queueUpdate();
		});
	}

	void registerOutputEvents() override
	{
		AbstractPipe::registerOutputEvents();
	}

public:
	std::vector<AbstractDataPort*> const& getOutputDataPorts() const
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