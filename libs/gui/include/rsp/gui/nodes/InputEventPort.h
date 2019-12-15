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

public:
	void draw() final override;
	ImVec2 calculateSize() const final override;
	void drawLink();
};

} // namespace rsp::gui