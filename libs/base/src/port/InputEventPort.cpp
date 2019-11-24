#include "rshp/base/port/InputEventPort.h"
#include "rshp/base/port/OutputEventPort.h"

namespace rshp::base
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

} // namespace rshp::base