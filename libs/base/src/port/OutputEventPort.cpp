#include "rshp/base/port/OutputEventPort.h"
#include "rshp/base/port/InputEventPort.h"

namespace rshp::base
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

} // namespace rshp::base