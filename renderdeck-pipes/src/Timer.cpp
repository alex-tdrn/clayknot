#include "renderdeck/pipes/Timer.hpp"

void Timer::registerInputEvents()
{
	AbstractSink::registerInputEvents();
	registerInputEvent(InputEvents::Poll, "Poll", [this](){
		this->poll();
	});
}

void Timer::registerOutputEvents()
{
	AbstractSink::registerOutputEvents();
	registerOutputEvent(OutputEvents::Timeout, "Timeout");
}

void Timer::update()
{
	poll();
}

void Timer::poll()
{
	if(std::chrono::steady_clock::now() >= nextActivationTime)
	{
		trigger("Timeout");
		nextActivationTime = std::chrono::steady_clock::now() + 5'000ms;//getInputDataPort<InputPorts::Interval>().getData();
	}
}
