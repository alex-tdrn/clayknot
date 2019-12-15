#pragma once

#include "rsp/gui/nodes/InputDataPort.h"
#include "rsp/gui/nodes/InputEventPort.h"
#include "rsp/gui/nodes/OutputDataPort.h"
#include "rsp/gui/nodes/OutputEventPort.h"

#include <imgui_node_editor.h>
#include <memory>
#include <vector>

namespace rsp
{
class AbstractNode;
}

namespace rsp::gui
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
	rsp::AbstractNode* node = nullptr;
	std::vector<std::unique_ptr<InputDataPort>> inputDataPorts;
	std::vector<std::unique_ptr<OutputDataPort>> outputDataPorts;
	std::vector<std::unique_ptr<InputEventPort>> inputEventPorts;
	std::vector<std::unique_ptr<OutputEventPort>> outputEventPorts;
	bool ran = false;
	Animation<float> borderWidth;

public:
	Node() = default;
	Node(rsp::AbstractNode* element);
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

} // namespace rsp::gui