#pragma once

#include "rsp/base/Port.hpp"
#include "rsp/gui/nodes/AbstractPort.hpp"

#include <imgui.h>

namespace rsp::gui
{
class OutputPort : public AbstractPort
{
public:
	OutputPort() = default;
	explicit OutputPort(rsp::Port* port);
	OutputPort(OutputPort&&) = default;
	OutputPort(OutputPort const&) = delete;
	auto operator=(OutputPort&&) -> OutputPort& = default;
	auto operator=(OutputPort const&) -> OutputPort& = delete;
	~OutputPort() override = default;

	auto canConnect(AbstractPort const* inputPort) const -> bool final;
	void connect(AbstractPort* inputPort) final;

private:
	auto calculateAnchorPosition() const -> ImVec2 override;
	void drawContents() override;
};

} // namespace rsp::gui