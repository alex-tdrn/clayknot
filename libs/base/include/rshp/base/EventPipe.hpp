#pragma once

#include "rshp/base/OutputEventPort.hpp"
#include "rshp/base/InputEventPort.hpp"

#include <unordered_map>
#include <cassert>
#include <memory>

namespace rshp::base
{
	
	class EventPipe
	{
	private:
		mutable bool inputEventsRegistered = false;
		mutable std::unordered_map<int, std::unique_ptr<InputEventPort>> inputEvents;
		mutable std::unordered_map<std::string, int> inputEventNames;
		mutable bool outputEventsRegistered = false;
		mutable std::unordered_map<int, OutputEventPort> outputEvents;
		mutable std::unordered_map<std::string, int> outputEventNames;

	public:
		EventPipe() = default;
		EventPipe(EventPipe const&) = delete;
		EventPipe(EventPipe&&) = default;
		EventPipe& operator=(EventPipe const&) = delete;
		EventPipe& operator=(EventPipe&&) = default;
		virtual ~EventPipe() = default;

	public:
		virtual void registerInputEvents() = 0;
		virtual void registerOutputEvents() = 0;

		template<typename F>
		void registerInputEvent(int index, std::string name, F&& callable)
		{
			if(inputEventNames.find(name) != inputEventNames.end() && inputEvents.find(index) != inputEvents.end())
				return;
			inputEventNames[name] = index;
			inputEvents[index] = std::make_unique<InputEventPort>(std::forward<F>(callable));
			inputEvents[index]->setName(name);
		}

		void registerOutputEvent(int index, std::string name)
		{
			if(outputEventNames.find(name) != outputEventNames.end() && outputEvents.find(index) != outputEvents.end())
				return;
			outputEventNames[name] = index;
			outputEvents[index] = OutputEventPort{};
			outputEvents[index].setName(name);
		}

		auto& getInputEventPorts()
		{
			if(!inputEventsRegistered)
			{
				registerInputEvents();
				inputEventsRegistered = true;
			}
			return inputEvents;
		}

		InputEventPort& getInputEventPort(int index)
		{
			if(!inputEventsRegistered)
			{
				registerInputEvents();
				inputEventsRegistered = true;
			}
			return *inputEvents[index];
		}

		InputEventPort& getInputEventPort(std::string name)
		{
			return getInputEventPort(inputEventNames[name]);
		}

		auto& getOutputEventPorts()
		{
			if(!outputEventsRegistered)
			{
				registerOutputEvents();
				outputEventsRegistered = true;
			}
			return outputEvents;
		}

		OutputEventPort& getOutputEventPort(int index)
		{
			if(!outputEventsRegistered)
			{
				registerOutputEvents();
				outputEventsRegistered = true;
			}

			return outputEvents[index];
		}

		OutputEventPort& getOutputEventPort(std::string name)
		{
			return getOutputEventPort(outputEventNames[name]);
		}

		void trigger(int index)
		{
			getOutputEventPort(index).trigger();
		}

		void trigger(std::string outputPortName)
		{
			getOutputEventPort(outputPortName).trigger();
		}

	};

}