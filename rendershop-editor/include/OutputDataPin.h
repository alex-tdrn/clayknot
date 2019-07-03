#pragma once
#include "OutputPin.h"
#include "OutputDataPinDelegate.h"

#include <memory>

class AbstractDataPort;
class OutputDataPinDelegate;

class OutputDataPin final : public OutputPin
{
private:
	AbstractDataPort* port;
	std::unique_ptr<OutputDataPinDelegate> delegate;

public:
	OutputDataPin() = default;
	OutputDataPin(AbstractDataPort* port);
	OutputDataPin(OutputDataPin&&) = default;
	OutputDataPin(OutputDataPin const&) = delete;
	OutputDataPin& operator=(OutputDataPin&&) = default;
	OutputDataPin& operator=(OutputDataPin const&) = delete;
	~OutputDataPin() = default;

public:
	void draw() override;
	ImVec2 calculateSize() const override;

};