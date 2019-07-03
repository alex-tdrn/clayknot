#pragma once
#include "OutputPin.h"

class OutputEventPort;

class OutputEventPin final : public OutputPin
{
private:
	OutputEventPort* port;
	int triggerCount = 0;

public:
	OutputEventPin() = default;
	OutputEventPin(OutputEventPort* port);
	OutputEventPin(OutputEventPin&&) = default;
	OutputEventPin(OutputEventPin const&) = delete;
	OutputEventPin& operator=(OutputEventPin&&) = default;
	OutputEventPin& operator=(OutputEventPin const&) = delete;
	~OutputEventPin() = default;

public:
	void draw() override;
	ImVec2 calculateSize() const override;

};