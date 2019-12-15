#pragma once

#include "rsp/base/port/EventPort.hpp"
#include "rsp/base/port/OutputPort.hpp"

namespace rsp
{
class InputEventPort;

class OutputEventPort final
	: public OutputPort<InputEventPort>
	, public EventPort
{
public:
	OutputEventPort() = default;
	OutputEventPort(OutputEventPort const&) = delete;
	OutputEventPort(OutputEventPort&&) = default;
	OutputEventPort& operator=(OutputEventPort const& that) = delete;
	OutputEventPort& operator=(OutputEventPort&&) = default;
	~OutputEventPort() = default;

public:
	void trigger() const;
	void operator()() const;
};

} // namespace rsp