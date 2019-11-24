#pragma once

#include "rshp/gui/panels/Panel.h"
#include "rshp/gui/nodes/Node.h"

#include <vector>
#include <memory>

namespace rshp::base
{
	class AbstractNode;
}

namespace rshp::gui
{

	class NodeEditor final : public Panel
	{
	private:
		ax::NodeEditor::EditorContext* context;
		std::vector<std::unique_ptr<rshp::base::AbstractNode>>* store = nullptr;
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
		void setStore(std::vector<std::unique_ptr<rshp::base::AbstractNode>>* store);

	};

}