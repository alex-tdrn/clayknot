#pragma once

#include "rsp/gui/nodes/OutputDataPortDelegate.h"
#include "rsp/gui/nodes/OutputPort.h"

#include <memory>

namespace rsp
{
class DataPort;
}

namespace rsp::gui
{
class OutputDataPortDelegate;

class OutputDataPort final : public OutputPort
{
private:
	rsp::DataPort* port;
	std::unique_ptr<OutputDataPortDelegate> delegate;

public:
	OutputDataPort() = default;
	OutputDataPort(rsp::DataPort* port);
	OutputDataPort(OutputDataPort&&) = default;
	OutputDataPort(OutputDataPort const&) = delete;
	OutputDataPort& operator=(OutputDataPort&&) = default;
	OutputDataPort& operator=(OutputDataPort const&) = delete;
	~OutputDataPort() = default;

public:
	void draw() override;
	ImVec2 calculateSize() const override;
};
} // namespace rsp::gui