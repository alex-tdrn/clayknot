#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/gui/Animation.hpp"
#include "rsp/gui/nodes/InputPort.hpp"
#include "rsp/gui/nodes/OutputPort.hpp"

#include <glm/glm.hpp>
#include <imgui_node_editor.h>
#include <memory>
#include <vector>

namespace rsp::gui
{
class Node
{
public:
	Node() = default;
	explicit Node(rsp::Node* node);
	Node(Node&&) = default;
	Node(Node const&) = delete;
	auto operator=(Node&&) -> Node& = default;
	auto operator=(Node const&) -> Node& = delete;
	~Node() = default;

	void setInputWidgetsVisibility(bool visibility);
	void setOutputWidgetsVisibility(bool visibility);
	auto countVisibleInputWidgets() const -> int;
	auto countVisibleOutputWidgets() const -> int;
	auto countHiddenInputWidgets() const -> int;
	auto countHiddenOutputWidgets() const -> int;
	void draw();
	void drawInputLinks();

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

	auto hasInputs() const -> bool;
	auto hasOutputs() const -> bool;
	void calculateLayout();
	void drawTitle();
	void drawInputs();
	void drawOutputs();
	void toggleInputWidgets();
	void toggleOutputWidgets();
	void toggleAllWidgets();
};

} // namespace rsp::gui