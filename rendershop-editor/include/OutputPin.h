#pragma once
#include "AbstractPin.hpp"

class OutputPin : public AbstractPin
{
public:
	OutputPin() = default;
	OutputPin(AbstractPort* port);
	OutputPin(OutputPin&&) = default;
	OutputPin(OutputPin const&) = delete;
	OutputPin& operator=(OutputPin&&) = default;
	OutputPin& operator=(OutputPin const&) = delete;
	virtual ~OutputPin() = default;

public:
	bool canConnect(AbstractPin* inputPin) final override;
	void connect(AbstractPin* inputPin) final override;

};