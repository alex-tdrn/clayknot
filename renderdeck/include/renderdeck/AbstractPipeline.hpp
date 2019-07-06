#pragma once
#include "renderdeck/Timestamp.hpp"

class AbstractPipelineElement
{
protected:
	virtual void update() const = 0;	

};

class AbstractSource : public AbstractPipelineElement
{
protected:
	mutable Timestamp timestamp;

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

class AbstractSink : public AbstractPipelineElement
{
public:
	virtual void trigger() const
	{
		update();
	}
};