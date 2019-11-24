#pragma once

#include "rshp/gui/nodes/OutputPort.h"

namespace rshp::base
{
class OutputEventPort;
}

namespace rshp::gui
{
class OutputEventPort final : public OutputPort
{
private:
	rshp::base::OutputEventPort* port;
	int triggerCount = 0;

public:
	OutputEventPort() = default;
	OutputEventPort(rshp::base::OutputEventPort* port);
	OutputEventPort(OutputEventPort&&) = default;
	OutputEventPort(OutputEventPort const&) = delete;
	OutputEventPort& operator=(OutputEventPort&&) = default;
	OutputEventPort& operator=(OutputEventPort const&) = delete;
	~OutputEventPort() = default;

public:
	void draw() override;
	ImVec2 calculateSize() const override;
};

} // namespace rshp::gui