#pragma once
#include "rshp/base/Sink.hpp"

#include <chrono>
#include <array>

using namespace std::chrono_literals;

class Timer : public Sink<Timer, InputList<std::chrono::milliseconds>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {
			"Interval"
		};
		enum
		{
			Interval
		};
	};

	struct InputEvents
	{
		enum
		{
			Poll = AbstractPipe::InputEvents::UserEvents
		};
	};

	struct OutputEvents
	{
		enum
		{
			Timeout = AbstractPipe::OutputEvents::UserEvents
		};
	};

public:
	static inline std::string const name = registerPipe<Timer>("Timer");

private:
	mutable std::chrono::steady_clock::time_point nextActivationTime = std::chrono::steady_clock::now() + 5'000ms;

protected:
	void registerInputEvents() override;
	void registerOutputEvents() override;

public:
	void update() override;
	void poll();

};