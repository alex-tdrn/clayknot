#pragma once

#include "rshp/base/node/AbstractNode.hpp"
#include "rshp/base/node/EventNode.hpp"
#include "rshp/base/port/DataPort.hpp"

namespace rshp::base
{
	
	class AbstractSink : public virtual AbstractNode
	{
	public:
		struct InputEvents
		{
			enum
			{
				Run = AbstractNode::InputEvents::SinkEvents
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
		virtual void updateAllInputs() const = 0;

		bool allInputsConnected() const
		{
			bool ret = true;
			for(auto inputDataPort : abstractInputDataPorts)
			{
				if(!inputDataPort->isConnected())
				{
					inputDataPort->requestFailed();
					ret = false;
				}
			}
			return ret;
		}

		void registerInputEvents() override
		{
			AbstractNode::registerInputEvents();
			registerInputEvent(InputEvents::Run, "Run", [this]() {
				run();
			});
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

		virtual void run() override
		{
			if(!allInputsConnected())
				return;

			updateAllInputs();
			update();
			trigger(AbstractNode::OutputEvents::Updated);
		}

	};

}