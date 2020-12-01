#pragma once

#include "rsp/base/Port.hpp"
#include "rsp/gui/nodes/AbstractPort.hpp"

#include <imgui.h>
#include <imgui_node_editor.h>

namespace rsp::gui
{
class OutputPort;

class InputPort : public AbstractPort
{
private:
	OutputPort* connection = nullptr;
	ax::NodeEditor::LinkId linkID = -1;

public:
	InputPort() = default;
	explicit InputPort(rsp::Port* port);
	InputPort(InputPort&&) = default;
	InputPort(InputPort const&) = delete;
	auto operator=(InputPort&&) -> InputPort& = default;
	auto operator=(InputPort const&) -> InputPort& = delete;
	~InputPort() override = default;

private:
	auto calculateAnchorPosition() const -> ImVec2 override;
	void generateViewer();
	void drawContents() override;

public:
	void drawLink();
	auto canConnect(AbstractPort const* outputPort) const -> bool final;
	void connect(AbstractPort* outputPort) final;
};

} // namespace rsp::gui