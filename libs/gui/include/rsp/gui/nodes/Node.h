#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/gui/nodes/InputPort.h"
#include "rsp/gui/nodes/OutputPort.h"

#include <imgui_node_editor.h>
#include <memory>
#include <vector>

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
	rsp::Node* node = nullptr;
	std::vector<std::unique_ptr<gui::InputPort>> inputPorts;
	std::vector<std::unique_ptr<gui::OutputPort>> outputPorts;
	bool ran = false;
	Animation<float> borderWidth;
	glm::vec2 size{0, 0};

public:
	Node() = default;
	Node(rsp::Node* element);
	Node(Node&&) = default;
	Node(Node const&) = delete;
	Node& operator=(Node&&) = default;
	Node& operator=(Node const&) = delete;
	~Node() = default;

private:
	bool hasInputs() const;
	bool hasOutputs() const;
	void calculateLayout();
	void drawTitle();
	void drawInputs();
	void drawOutputs();
	void toggleInputWidgets();
	void toggleOutputWidgets();
	void toggleAllWidgets();

public:
	void setInputWidgetsVisibility(bool visibility);
	void setOutputWidgetsVisibility(bool visibility);
	int countVisibleInputWidgets() const;
	int countVisibleOutputWidgets() const;
	int countHiddenInputWidgets() const;
	int countHiddenOutputWidgets() const;
	void draw();
	void drawInputLinks();
};

} // namespace rsp::gui