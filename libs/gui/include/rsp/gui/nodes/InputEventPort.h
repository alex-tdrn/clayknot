#pragma once

#include "rsp/gui/nodes/InputPort.h"

namespace rsp
{
class InputEventPort;
}

namespace rsp::gui
{
class InputEventPort final : public InputPort
{
private:
	rsp::InputEventPort* port;
	bool triggered = false;

public:
	InputEventPort() = default;
	InputEventPort(rsp::InputEventPort* port);
	InputEventPort(InputEventPort&&) = default;
	InputEventPort(InputEventPort const&) = delete;
	InputEventPort& operator=(InputEventPort&&) = default;
	InputEventPort& operator=(InputEventPort const&) = delete;
	~InputEventPort() = default;

private:
	void drawContents() override;

public:
	void drawLink();
};

} // namespace rsp::gui