#pragma once
#include "InputPin.h"

namespace rshp::base
{
	class InputEventPort;
}

class InputEventPin final : public InputPin
{
private:
	rshp::base::InputEventPort* port;
	int triggerCount = 0;
	bool justTriggered = false;

public:
	InputEventPin() = default;
	InputEventPin(rshp::base::InputEventPort* port);
	InputEventPin(InputEventPin&&) = default;
	InputEventPin(InputEventPin const&) = delete;
	InputEventPin& operator=(InputEventPin&&) = default;
	InputEventPin& operator=(InputEventPin const&) = delete;
	~InputEventPin() = default;

public:
	void draw() final override;
	ImVec2 calculateSize() const final override;
	void drawLink();

};