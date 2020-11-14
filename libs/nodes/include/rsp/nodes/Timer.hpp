#pragma once

#include "rsp/base/Node.hpp"

#include <array>
#include <chrono>

namespace rsp::nodes
{
using namespace std::chrono_literals;

class Timer final : public Node
{
private:
	InputPort<std::chrono::milliseconds> interval{"Interval"};

public:
	Timer()
	{
		registerPort(interval);
	}

public:
	void update() override;

public:
	std::string const& getName() const override
	{
		static std::string name = "Timer";
		return name;
	}
};

} // namespace rsp::nodes