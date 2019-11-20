#pragma once
#include "AbstractPort.hpp"
#include "Animation.hpp"

class OutputPort : public AbstractPort
{
public:
	OutputPort() = default;
	OutputPort(rshp::base::Port* port);
	OutputPort(OutputPort&&) = default;
	OutputPort(OutputPort const&) = delete;
	OutputPort& operator=(OutputPort&&) = default;
	OutputPort& operator=(OutputPort const&) = delete;
	virtual ~OutputPort() = default;

protected:
	ImVec2 calculateAnchorPosition() const override;

public:
	bool canConnect(AbstractPort* inputPort) final override;
	void connect(AbstractPort* inputPort) final override;

};