#include "rsp/base/port/OutputEventPort.h"
#include "rsp/base/port/InputEventPort.h"

namespace rsp
{
int OutputEventPort::getTimesTriggered() const
{
	return timesTriggered;
}

void OutputEventPort::trigger() const
{
	for(auto connection : connections)
		connection->trigger();
	timesTriggered++;
}
void OutputEventPort::operator()() const
{
	trigger();
}

} // namespace rsp