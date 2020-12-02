#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/gui/nodes/Node.h"
#include "rsp/gui/panels/Panel.h"

#include <imgui_node_editor.h>
#include <memory>
#include <vector>

namespace rsp::gui
{
class NodeEditor final : public Panel
{
public:
	NodeEditor();
	NodeEditor(NodeEditor const&) = delete;
	NodeEditor(NodeEditor&&) = default;
	auto operator=(NodeEditor const&) -> NodeEditor& = delete;
	auto operator=(NodeEditor&&) -> NodeEditor& = delete;
	~NodeEditor() override;

	void setStore(std::vector<std::unique_ptr<rsp::Node>>* store);

private:
	ax::NodeEditor::EditorContext* context;
	std::vector<std::unique_ptr<rsp::Node>>* store = nullptr;
	std::vector<std::unique_ptr<Node>> nodes;
	bool showInputWidgets = true;
	bool showOutputWidgets = true;

	void toggleInputWidgets();
	void toggleOutputWidgets();
	void toggleAllWidgets();
	void drawContents() override;
};

} // namespace rsp::gui