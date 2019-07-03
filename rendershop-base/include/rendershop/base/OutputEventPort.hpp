#pragma once

#include "rendershop/base/OutputPort.hpp"
#include "rendershop/base/InputEventPort.hpp"

class InputEventPort;

class OutputEventPort final :  public OutputPort<InputEventPort>
{
private:
	mutable int timesTriggered = 0;

public:
	OutputEventPort() = default;
	OutputEventPort(OutputEventPort const&) = delete;
	OutputEventPort(OutputEventPort&&) = default;
	OutputEventPort& operator=(OutputEventPort const& that) = delete;
	OutputEventPort& operator=(OutputEventPort&&) = default;
	~OutputEventPort() = default;

public:
	int getTimesTriggered() const
	{
		return timesTriggered;
	}

	void trigger() const
	{
		for(auto connection : connections)
			connection->trigger();
		timesTriggered++;
	}
	void operator()() const
	{
		trigger();
	}

};