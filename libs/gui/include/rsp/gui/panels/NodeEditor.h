#pragma once

#include "rsp/gui/nodes/Node.h"
#include "rsp/gui/panels/Panel.h"

#include <memory>
#include <vector>

namespace rsp
{
class AbstractNode;
}

namespace rsp::gui
{
class NodeEditor final : public Panel
{
private:
	ax::NodeEditor::EditorContext* context;
	std::vector<std::unique_ptr<rsp::AbstractNode>>* store = nullptr;
	std::vector<Node> nodes;

public:
	NodeEditor();
	NodeEditor(NodeEditor const&) = delete;
	NodeEditor(NodeEditor&&) = default;
	NodeEditor& operator=(NodeEditor const&) = delete;
	NodeEditor& operator=(NodeEditor&&) = default;
	~NodeEditor();

private:
	void drawContents() override;

public:
	void setStore(std::vector<std::unique_ptr<rsp::AbstractNode>>* store);
};

} // namespace rsp::gui