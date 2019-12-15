#include "rsp/base/port/OutputEventPort.h"
#include "rsp/base/port/InputEventPort.h"

namespace rsp
{
void OutputEventPort::trigger() const
{
	for(auto connection : connections)
		connection->trigger();
	if(connections.empty())
		notifyObserverFlags(EventPort::ObserverFlags::onTriggerFailed);
	else
		notifyObserverFlags(EventPort::ObserverFlags::onTriggered);
}
void OutputEventPort::operator()() const
{
	trigger();
}

} // namespace rsp