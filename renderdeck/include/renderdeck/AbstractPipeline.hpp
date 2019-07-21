#pragma once
#include "renderdeck/Timestamp.hpp"
#include "renderdeck/ResourcePort.hpp"

#include <string>
#include <vector>

class AbstractPipelineElement
{
protected:
	std::vector<AbstractResourcePort*> abstractInputPorts;
	std::vector<AbstractResourcePort*> abstractOutputPorts;

protected:
	virtual void update() const = 0;

public:
	virtual std::string const& getTypeName() const = 0;
	std::vector<AbstractResourcePort*> const& getAbstractInputPorts() const
	{
		return abstractInputPorts;
	}

	std::vector<AbstractResourcePort*> const& getAbstractOutputPorts() const
	{
		return abstractOutputPorts;
	}

};

class AbstractSource : public virtual AbstractPipelineElement
{
protected:
	mutable Timestamp timestamp;

public:
	AbstractSource() = default;
	virtual ~AbstractSource() = default;

protected:
	bool isUpdateQueued() const
	{
		return timestamp.isReset();
	}

public:
	Timestamp const& getTimestamp() const
	{
		return timestamp;
	}

	void queueUpdate()
	{
		timestamp.reset();
	}

	virtual void updateOutputsIfNeeded() const = 0;

};

class AbstractSink : public virtual AbstractPipelineElement
{
protected:
	virtual void updateAllInputs() const = 0;

public:
	virtual bool allInputsConnected() const = 0;

	virtual void trigger() const
	{
		if(!allInputsConnected())
			return;

		updateAllInputs();
		update();
	}
};