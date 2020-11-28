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
	InputPort(rsp::Port* port);
	InputPort(InputPort&&) = default;
	InputPort(InputPort const&) = delete;
	InputPort& operator=(InputPort&&) = default;
	InputPort& operator=(InputPort const&) = delete;
	virtual ~InputPort() = default;

private:
	ImVec2 calculateAnchorPosition() const override;
	void generateViewer();
	void drawContents() override;

public:
	void drawLink();
	bool canConnect(AbstractPort const* outputPort) const final override;
	void connect(AbstractPort* outputPort) final override;
};

} // namespace rsp::gui