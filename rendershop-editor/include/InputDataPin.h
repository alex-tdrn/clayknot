#pragma once
#include "InputPin.h"

class AbstractDataPort;

class InputDataPin final : public InputPin
{
private:
	AbstractDataPort* port;
	int dataRequests = 0;
	bool dataRequested = false;

public:
	InputDataPin() = default;
	InputDataPin(AbstractDataPort* port);
	InputDataPin(InputDataPin&&) = default;
	InputDataPin(InputDataPin const&) = delete;
	InputDataPin& operator=(InputDataPin&&) = default;
	InputDataPin& operator=(InputDataPin const&) = delete;
	~InputDataPin() = default;

public:
	void draw() final override;
	ImVec2 calculateSize() const final override;
	void drawLink();

};