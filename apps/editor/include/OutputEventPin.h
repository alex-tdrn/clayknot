#pragma once
#include "OutputPin.h"

namespace rshp::base
{
	class OutputEventPort;
}

class OutputEventPin final : public OutputPin
{
private:
	rshp::base::OutputEventPort* port;
	int triggerCount = 0;

public:
	OutputEventPin() = default;
	OutputEventPin(rshp::base::OutputEventPort* port);
	OutputEventPin(OutputEventPin&&) = default;
	OutputEventPin(OutputEventPin const&) = delete;
	OutputEventPin& operator=(OutputEventPin&&) = default;
	OutputEventPin& operator=(OutputEventPin const&) = delete;
	~OutputEventPin() = default;

public:
	void draw() override;
	ImVec2 calculateSize() const override;

};