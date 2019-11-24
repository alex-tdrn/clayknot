#pragma once
#include "rshp/gui/nodes/InputDataPort.h"
#include "rshp/gui/nodes/OutputDataPort.h"
#include "rshp/gui/nodes/InputEventPort.h"
#include "rshp/gui/nodes/OutputEventPort.h"

#include <imgui_node_editor.h>
#include <vector>
#include <memory>

namespace rshp::base
{
	class AbstractNode;
}

namespace rshp::gui
{

	class Node
	{
	private:
		bool layoutInitialized = false;
		float outputsWidth = 0;
		float contentsWidth = 0;
		float titleOffset = 0;
		float centerSpacing = 0;
		ax::NodeEditor::NodeId id = -1;
		rshp::base::AbstractNode* node = nullptr;
		std::vector<std::unique_ptr<InputDataPort>> inputDataPorts;
		std::vector<std::unique_ptr<OutputDataPort>> outputDataPorts;	
		std::vector<std::unique_ptr<InputEventPort>> inputEventPorts;
		std::vector<std::unique_ptr<OutputEventPort>> outputEventPorts;

	public:
		Node() = default;
		Node(rshp::base::AbstractNode* element);
		Node(Node&&) = default;
		Node(Node const&) = delete;
		Node& operator=(Node&&) = default;
		Node& operator=(Node const&) = delete;
		~Node() = default;

	private:
		bool hasInputs() const;
		bool hasOutputs() const;
		void initializeLayout();
		void drawTitle();
		void drawInputs();
		void drawOutputs();

	public:
		void draw();
		void drawInputLinks();

	};

}