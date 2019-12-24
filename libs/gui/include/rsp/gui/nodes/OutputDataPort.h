#pragma once

#include "rsp/gui/nodes/DataPort.hpp"
#include "rsp/gui/nodes/OutputPort.h"
#include "rsp/gui/widgets/Widget.hpp"

#include <memory>

namespace rsp::gui
{
class Widget;

class OutputDataPort final
	: public OutputPort
	, public DataPort
{
public:
	OutputDataPort() = default;
	OutputDataPort(rsp::DataPort* port, bool fixedNode = false);
	OutputDataPort(OutputDataPort&&) = default;
	OutputDataPort(OutputDataPort const&) = delete;
	OutputDataPort& operator=(OutputDataPort&&) = default;
	OutputDataPort& operator=(OutputDataPort const&) = delete;
	~OutputDataPort() = default;

private:
	void drawContents() override;
};
} // namespace rsp::gui