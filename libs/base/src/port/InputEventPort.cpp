#include "rsp/base/port/InputEventPort.h"
#include "rsp/base/port/OutputEventPort.h"

namespace rsp
{
void InputEventPort::trigger() const
{
	f();
	notifyObserverFlags(EventPort::ObserverFlags::onTriggered);
}

void InputEventPort::operator()() const
{
	trigger();
}

} // namespace rsp