#pragma once
#include "AbstractPin.hpp"
#include "Animation.hpp"

class OutputPin : public AbstractPin
{
public:
	OutputPin() = default;
	OutputPin(rshp::base::AbstractPort* port);
	OutputPin(OutputPin&&) = default;
	OutputPin(OutputPin const&) = delete;
	OutputPin& operator=(OutputPin&&) = default;
	OutputPin& operator=(OutputPin const&) = delete;
	virtual ~OutputPin() = default;

protected:
	ImVec2 calculateAnchorPosition() const override;

public:
	bool canConnect(AbstractPin* inputPin) final override;
	void connect(AbstractPin* inputPin) final override;

};