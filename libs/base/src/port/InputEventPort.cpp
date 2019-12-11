#include "rsp/base/port/InputEventPort.h"
#include "rsp/base/port/OutputEventPort.h"

namespace rsp
{
int InputEventPort::getTimesTriggered() const
{
	return timesTriggered;
}

void InputEventPort::trigger() const
{
	f->call();
	timesTriggered++;
}

void InputEventPort::operator()() const
{
	trigger();
}

} // namespace rsp