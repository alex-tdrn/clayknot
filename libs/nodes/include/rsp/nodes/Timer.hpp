#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/base/Port.hpp"

#include <chrono>
#include <string>
#include <unordered_set>

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
	auto getName() const -> std::string const& override
	{
		static std::string name = "Timer";
		return name;
	}
};

} // namespace rsp::nodes