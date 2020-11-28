#include "rsp/gui/nodes/Node.h"
#include "rsp/gui/Stylesheet.hpp"
#include "rsp/gui/UniqueID.hpp"
#include "rsp/util/Bounded.hpp"

#include <algorithm>
#include <chrono>
#include <imgui.h>
#include <string>

namespace rsp::gui
{
Node::Node(rsp::Node* node) : node(node), id(uniqueID())
{
	bool isSupportedFixedSource = false;

	for(auto inputPort : node->getInputPorts())
		inputPorts.push_back(std::make_unique<gui::InputPort>(inputPort));
	for(auto outputPort : node->getOutputPorts())
		outputPorts.push_back(std::make_unique<gui::OutputPort>(outputPort));
}

bool Node::hasInputs() const
{
	return !inputPorts.empty();
}

bool Node::hasOutputs() const
{
	return !outputPorts.empty();
}

void Node::calculateLayout()
{
	outputsWidth = 0;
	contentsWidth = 0;
	titleOffset = 0;
	centerSpacing = 0;

	float const itemSpacing = ImGui::GetStyle().ItemSpacing.x;
	float const minimumCenterSpacing = 20;

	auto measurePinGroup = [&](auto& dataPins) {
		float pinGroupWidth = 0;
		for(auto const& pin : dataPins)
			pinGroupWidth = std::max(pinGroupWidth, pin->getSize().x);

		if(dataPins.size() > 0)
			contentsWidth += pinGroupWidth + itemSpacing * 2;
		return pinGroupWidth;
	};

	measurePinGroup(inputPorts);
	outputsWidth = measurePinGroup(outputPorts);

	float const titleWidth = ImGui::CalcTextSize(node->getName().c_str()).x;

	if(titleWidth > contentsWidth + minimumCenterSpacing)
	{
		centerSpacing = titleWidth - contentsWidth;
	}
	else
	{
		centerSpacing = minimumCenterSpacing;
		titleOffset = (contentsWidth + centerSpacing - titleWidth) / 2;
	}
	contentsWidth += centerSpacing;

	layoutInitialized = true;
}

void Node::drawTitle()
{
	auto const& nodeEditorStyle = ax::NodeEditor::GetStyle();
	ImVec2 min = ImGui::GetCursorPos();
	min.x += ImGui::GetWindowPos().x;
	min.y += ImGui::GetWindowPos().y;

	ImVec2 max = min;
	min.x -= nodeEditorStyle.NodePadding.x - nodeEditorStyle.NodeBorderWidth;
	min.y -= nodeEditorStyle.NodePadding.y - nodeEditorStyle.NodeBorderWidth;
	max.x += nodeEditorStyle.NodePadding.z - nodeEditorStyle.NodeBorderWidth;

	auto const& coreStyle = ImGui::GetStyle();
	max.x += contentsWidth;
	max.y += ImGui::GetTextLineHeight();
	auto color = ImGui::GetColorU32(coreStyle.Colors[ImGuiCol_TitleBgActive]);
	auto drawList = ax::NodeEditor::GetNodeBackgroundDrawList(id);
	drawList->AddRectFilled(min, max, color, nodeEditorStyle.NodeRounding, ImDrawCornerFlags_Top);

	if(titleOffset > 0)
		ImGui::Indent(titleOffset);

	ImGui::Text(node->getName().c_str());

	if(titleOffset > 0)
		ImGui::Unindent(titleOffset);

	if(ImGui::IsMouseHoveringRect(min, max) && ImGui::IsMouseReleased(1))
	{
		float const titleMinX = ImGui::GetItemRectMin().x;
		float const titleWidth = ImGui::GetItemRectSize().x;

		if(ImGui::GetMousePos().x < titleMinX + titleWidth / 3)
			toggleInputWidgets();
		else if(ImGui::GetMousePos().x < titleMinX + 2 * titleWidth / 3)
			toggleAllWidgets();
		else
			toggleOutputWidgets();
	}
}

void Node::drawInputs()
{
	if(hasInputs())
	{
		ImGui::BeginGroup();
		for(auto& InputPort : inputPorts)
		{
			InputPort->draw();
			ImGui::SameLine();
			ImGui::Dummy({0, 0});
		}
		ImGui::EndGroup();
	}
}

void Node::drawOutputs()
{
	if(hasOutputs())
	{
		ImGui::BeginGroup();
		for(auto& OutputPort : outputPorts)
		{
			ImGui::Dummy({outputsWidth - OutputPort->getSize().x, 0});
			ImGui::SameLine();
			OutputPort->draw();
		}
		ImGui::EndGroup();
	}
}

void Node::toggleInputWidgets()
{
	setInputWidgetsVisibility(countVisibleInputWidgets() < countHiddenInputWidgets());
}

void Node::toggleOutputWidgets()
{
	setOutputWidgetsVisibility(countVisibleOutputWidgets() < countHiddenOutputWidgets());
}

void Node::toggleAllWidgets()
{
	int visibleCount = countVisibleInputWidgets() + countVisibleOutputWidgets();
	int hiddenCount = countHiddenInputWidgets() + countHiddenOutputWidgets();
	bool visibility = visibleCount < hiddenCount;
	setInputWidgetsVisibility(visibility);
	setOutputWidgetsVisibility(visibility);
}

void Node::setInputWidgetsVisibility(bool visibility)
{
	for(auto& port : inputPorts)
		port->setWidgetVisibility(visibility);
}

void Node::setOutputWidgetsVisibility(bool visibility)
{
	for(auto& port : outputPorts)
		port->setWidgetVisibility(visibility);
}

int Node::countVisibleInputWidgets() const
{
	int visibleCount = 0;
	for(auto& port : inputPorts)
		if(port->isWidgetVisible())
			visibleCount++;
	return visibleCount;
}

int Node::countVisibleOutputWidgets() const
{
	int visibleCount = 0;
	for(auto& port : outputPorts)
		if(port->isWidgetVisible())
			visibleCount++;
	return visibleCount;
}

int Node::countHiddenInputWidgets() const
{
	int hiddenCount = 0;
	for(auto& port : inputPorts)
		if(!port->isWidgetVisible())
			hiddenCount++;
	return hiddenCount;
}

int Node::countHiddenOutputWidgets() const
{
	int hiddenCount = 0;
	for(auto& port : outputPorts)
		if(!port->isWidgetVisible())
			hiddenCount++;
	return hiddenCount;
}

void Node::draw()
{
	if(!layoutInitialized)
		calculateLayout();
	auto& nodeEditorStyle = ax::NodeEditor::GetStyle();

	if(ran)
	{
		borderWidth.play();
		ran = false;
	}

	nodeEditorStyle.NodeBorderWidth = borderWidth.get(Stylesheet::getCurrentSheet().nodeBorderWidth * 5,
		Stylesheet::getCurrentSheet().nodeBorderWidth, 1s, AnimationCurve::linear);

	ax::NodeEditor::BeginNode(id);

	drawTitle();

	drawInputs();

	if(hasInputs())
		ImGui::SameLine();

	ImGui::Dummy({centerSpacing, 0});

	if(hasOutputs())
		ImGui::SameLine();

	drawOutputs();

	ax::NodeEditor::EndNode();
}

void Node::drawInputLinks()
{
	for(auto& InputPort : inputPorts)
		InputPort->drawLink();
}

} // namespace rsp::gui