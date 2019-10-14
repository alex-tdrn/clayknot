#pragma once

#include "renderdeck/base/AbstractPipe.hpp"
#include "renderdeck/base/EventPipe.hpp"
#include "renderdeck/base/AbstractDataPort.hpp"

class AbstractSink : public virtual AbstractPipe
{
public:
	struct InputEvents
	{
		enum
		{
			Run = AbstractPipe::InputEvents::SinkEvents
		};
	};

	struct OutputEvents
	{
		enum
		{
		};
	};

protected:
	std::vector<AbstractDataPort*> abstractInputDataPorts;

public:
	AbstractSink() = default;
	AbstractSink(AbstractSink const&) = delete;
	AbstractSink(AbstractSink&&) = default;
	AbstractSink& operator=(AbstractSink const&) = delete;
	AbstractSink& operator=(AbstractSink&&) = default;
	virtual ~AbstractSink() = default;


protected:
	virtual void updateAllInputs() const = 0;

	bool allInputsConnected() const
	{
		bool ret = true;
		for(auto inputDataPort : abstractInputDataPorts)
			ret &= inputDataPort->isConnected();
		return ret;
	}

	void registerInputEvents() override
	{
		AbstractPipe::registerInputEvents();
		registerInputEvent(InputEvents::Run, "Run", [this]() {
			run();
		});
	}

	void registerOutputEvents() override
	{
		AbstractPipe::registerOutputEvents();
	}

public:

	std::vector<AbstractDataPort*> const& getInputDataPorts() const
	{
		return abstractInputDataPorts;
	}

	virtual void run() override
	{
		if(!allInputsConnected())
			return;

		updateAllInputs();
		update();
		trigger(AbstractPipe::OutputEvents::Updated);
	}

};