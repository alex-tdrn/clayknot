#pragma once
#include "InputDataPin.h"
#include "OutputDataPin.h"
#include "InputEventPin.h"
#include "OutputEventPin.h"

#include <imgui_node_editor.h>
#include <vector>
#include <memory>

namespace rshp::base
{
	class AbstractPipe;
}

class Node
{
private:
	bool layoutInitialized = false;
	float outputsWidth = 0;
	float contentsWidth = 0;
	float titleOffset = 0;
	float centerSpacing = 0;
	ax::NodeEditor::NodeId id = -1;
	rshp::base::AbstractPipe* pipe = nullptr;
	std::vector<std::unique_ptr<InputDataPin>> inputDataPins;
	std::vector<std::unique_ptr<OutputDataPin>> outputDataPins;	
	std::vector<std::unique_ptr<InputEventPin>> inputEventPins;
	std::vector<std::unique_ptr<OutputEventPin>> outputEventPins;

public:
	Node() = default;
	Node(rshp::base::AbstractPipe* element);
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