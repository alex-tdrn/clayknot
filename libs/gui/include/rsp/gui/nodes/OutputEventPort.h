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
	bool triggerFailed = false;

public:
	OutputEventPort() = default;
	OutputEventPort(rsp::OutputEventPort* port);
	OutputEventPort(OutputEventPort&&) = default;
	OutputEventPort(OutputEventPort const&) = delete;
	OutputEventPort& operator=(OutputEventPort&&) = default;
	OutputEventPort& operator=(OutputEventPort const&) = delete;
	~OutputEventPort() = default;

private:
	void drawContents() override;
};

} // namespace rsp::gui