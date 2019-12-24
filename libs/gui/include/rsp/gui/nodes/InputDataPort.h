#pragma once

#include "rsp/gui/nodes/DataPort.hpp"
#include "rsp/gui/nodes/InputPort.h"
#include "rsp/gui/widgets/Widget.hpp"

namespace rsp::gui
{
class InputDataPort final
	: public InputPort
	, public DataPort
{
private:
	bool portUpdateFailed = false;
	bool dataRequested = false;
	bool connectionStateChanged = false;

public:
	InputDataPort() = default;
	InputDataPort(rsp::DataPort* port);
	InputDataPort(InputDataPort&&) = default;
	InputDataPort(InputDataPort const&) = delete;
	InputDataPort& operator=(InputDataPort&&) = default;
	InputDataPort& operator=(InputDataPort const&) = delete;
	~InputDataPort() = default;

private:
	void generateViewer();
	void drawContents() override;

public:
	void drawLink();
};

} // namespace rsp::gui