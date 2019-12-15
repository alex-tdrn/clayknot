#include "rsp/nodes/Timer.hpp"

namespace rsp::nodes
{
void Timer::registerInputEvents()
{
	AbstractSink::registerInputEvents();
	registerInputEvent(InputEvents::Poll, "Poll", [this]() { this->poll(); });
}

void Timer::registerOutputEvents()
{
	AbstractSink::registerOutputEvents();
	registerOutputEvent(OutputEvents::Timeout, "Timeout");
}

void Timer::run()
{
	poll();
}

void Timer::poll()
{
	if(std::chrono::steady_clock::now() >= nextActivationTime)
	{
		trigger(OutputEvents::Timeout);
		nextActivationTime = std::chrono::steady_clock::now() + 5'000ms;
		// TODO
		// getInputDataPort<InputPorts::Interval>().getData();
	}
}

} // namespace rsp::nodes