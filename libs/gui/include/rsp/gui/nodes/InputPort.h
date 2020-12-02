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
public:
	InputPort() = default;
	explicit InputPort(rsp::Port* port);
	InputPort(InputPort&&) = default;
	InputPort(InputPort const&) = delete;
	auto operator=(InputPort&&) -> InputPort& = default;
	auto operator=(InputPort const&) -> InputPort& = delete;
	~InputPort() override = default;

	void drawLink();
	auto canConnect(AbstractPort const* outputPort) const -> bool final;
	void connect(AbstractPort* outputPort) final;

private:
	OutputPort* connection = nullptr;
	ax::NodeEditor::LinkId linkID = -1;

	auto calculateAnchorPosition() const -> ImVec2 override;
	void generateViewer();
	void drawContents() override;
};

} // namespace rsp::gui