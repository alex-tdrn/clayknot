#pragma once

#include "rsp/base/node/Sink.hpp"

#include <array>
#include <chrono>

namespace rsp::nodes
{
using namespace std::chrono_literals;

class Timer : public rsp::Sink<Timer, rsp::InputList<std::chrono::milliseconds>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {"Interval"};
		enum
		{
			Interval
		};
	};

	struct InputEvents
	{
		enum
		{
			Poll = AbstractNode::InputEvents::UserEvents
		};
	};

	struct OutputEvents
	{
		enum
		{
			Timeout = AbstractNode::OutputEvents::UserEvents
		};
	};

public:
	static inline std::string const name = registerNode<Timer>("Timer");

private:
	mutable std::chrono::steady_clock::time_point nextActivationTime = std::chrono::steady_clock::now() + 5'000ms;

protected:
	void registerInputEvents() override;
	void registerOutputEvents() override;

public:
	void update() override;
	void poll();
};

} // namespace rsp::nodes