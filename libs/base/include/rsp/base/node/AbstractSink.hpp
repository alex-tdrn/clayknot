#pragma once

#include "rsp/base/node/AbstractNode.hpp"
#include "rsp/base/node/EventNode.hpp"
#include "rsp/base/port/DataPort.hpp"

namespace rsp
{
class AbstractSink : public virtual AbstractNode
{
public:
	struct InputEvents
	{
		enum
		{
			Update = AbstractNode::InputEvents::SinkEvents
		};
	};

	struct OutputEvents
	{
		enum
		{
		};
	};

protected:
	std::vector<DataPort*> abstractInputDataPorts;

public:
	AbstractSink() = default;
	AbstractSink(AbstractSink const&) = delete;
	AbstractSink(AbstractSink&&) = default;
	AbstractSink& operator=(AbstractSink const&) = delete;
	AbstractSink& operator=(AbstractSink&&) = default;
	virtual ~AbstractSink() = default;

protected:
	[[nodiscard]] virtual bool updateAllInputs() const = 0;

	void registerInputEvents() override
	{
		AbstractNode::registerInputEvents();
		registerInputEvent(InputEvents::Update, "Update", [this]() { update(); });
	}

	void registerOutputEvents() override
	{
		AbstractNode::registerOutputEvents();
	}

public:
	std::vector<DataPort*> const& getInputDataPorts() const
	{
		return abstractInputDataPorts;
	}

	[[nodiscard]] virtual bool update() override
	{
		if(!updateAllInputs())
			return false;

		run();
		notifyObserverFlags(AbstractNode::ObserverFlags::onRun);
		trigger(AbstractNode::OutputEvents::Ran);
		return true;
	}
};

} // namespace rsp