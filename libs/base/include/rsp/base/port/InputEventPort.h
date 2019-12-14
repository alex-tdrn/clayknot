#pragma once

#include "rsp/base/port/InputPort.hpp"
#include "rsp/base/port/OutputEventPort.h"

#include <functional>
#include <memory>

namespace rsp
{
class OutputEventPort;

class InputEventPort : public InputPort<OutputEventPort>
{
private:
	std::function<void()> f;
	mutable int timesTriggered = 0;

public:
	template <typename F>
	InputEventPort(F&& callable) : f(std::forward<F>(callable))
	{
	}
	InputEventPort(InputEventPort const&) = delete;
	InputEventPort(InputEventPort&&) = default;
	InputEventPort& operator=(InputEventPort const& that) = delete;
	InputEventPort& operator=(InputEventPort&&) = default;
	virtual ~InputEventPort() = default;

public:
	int getTimesTriggered() const;
	void trigger() const;
	void operator()() const;
};

} // namespace rsp