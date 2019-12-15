#pragma once

#include "rsp/base/node/Sink.hpp"
#include "rsp/base/node/Source.hpp"

namespace rsp
{
template <typename ConcreteNode, typename InputList, typename OutputList>
class Node
	: virtual public Sink<ConcreteNode, InputList>
	, virtual public Source<ConcreteNode, OutputList>
{
public:
	struct InputEvents
	{
		enum
		{
		};
	};

	struct OutputEvents
	{
		enum
		{
		};
	};

public:
	Node() = default;
	Node(Node const&) = delete;
	Node(Node&&) = default;
	Node& operator=(Node const&) = delete;
	Node& operator=(Node&&) = default;
	virtual ~Node() = default;

private:
	void registerInputEvents() override
	{
		AbstractSink::registerInputEvents();
		AbstractSource::registerInputEvents();
	}

	void registerOutputEvents() override
	{
		AbstractSink::registerOutputEvents();
		AbstractSource::registerOutputEvents();
	}

public:
	std::string const& getName() const override final
	{
		return ConcreteNode::name;
	}

	[[nodiscard]] bool update() override
	{
		if(!this->updateAllInputs())
			return false;

		if(!this->isRunQueued())
		{
			bool outputsOutdated = false;
			static_for(this->inputs.list, [&](auto const& input) {
				if(input.getTimestamp().isNewerThan(this->timestamp))
					outputsOutdated = true;
			});

			if(!outputsOutdated)
				return true;
		}

		this->run();
		notifyObserverFlags(AbstractNode::ObserverFlags::onRun);
		this->timestamp.update();
		this->trigger(AbstractNode::OutputEvents::Ran);
		return true;
	}
};

} // namespace rsp