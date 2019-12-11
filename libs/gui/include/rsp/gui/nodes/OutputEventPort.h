#pragma once

#include "rsp/gui/nodes/OutputPort.h"

namespace rsp
{
class OutputEventPort;
}

namespace rsp::gui
{
class OutputEventPort final : public OutputPort
{
private:
	rsp::OutputEventPort* port;
	int triggerCount = 0;

public:
	OutputEventPort() = default;
	OutputEventPort(rsp::OutputEventPort* port);
	OutputEventPort(OutputEventPort&&) = default;
	OutputEventPort(OutputEventPort const&) = delete;
	OutputEventPort& operator=(OutputEventPort&&) = default;
	OutputEventPort& operator=(OutputEventPort const&) = delete;
	~OutputEventPort() = default;

public:
	void draw() override;
	ImVec2 calculateSize() const override;
};

} // namespace rsp::gui