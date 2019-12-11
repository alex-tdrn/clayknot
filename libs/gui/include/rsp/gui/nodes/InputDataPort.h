#pragma once

#include "rsp/gui/nodes/InputPort.h"

namespace rsp
{
class DataPort;
}

namespace rsp::gui
{
class InputDataPort final : public InputPort
{
private:
	rsp::DataPort* port;
	int dataRequests = 0;
	bool dataRequested = false;

public:
	InputDataPort() = default;
	InputDataPort(rsp::DataPort* port);
	InputDataPort(InputDataPort&&) = default;
	InputDataPort(InputDataPort const&) = delete;
	InputDataPort& operator=(InputDataPort&&) = default;
	InputDataPort& operator=(InputDataPort const&) = delete;
	~InputDataPort() = default;

public:
	void draw() final override;
	ImVec2 calculateSize() const final override;
	void drawLink();
};

} // namespace rsp::gui