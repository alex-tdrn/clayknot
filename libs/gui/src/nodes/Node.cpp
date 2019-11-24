#include "rshp/gui/nodes/Node.h"
#include "rshp/base/node/AbstractSink.hpp"
#include "rshp/base/node/AbstractSource.hpp"
#include "rshp/gui/Stylesheet.hpp"
#include "rshp/gui/UIDebug.hpp"
#include "rshp/gui/UniqueID.hpp"

#include <algorithm>

namespace rshp::gui
{
Node::Node(rshp::base::AbstractNode* node) : node(node), id(uniqueID())
{
	if(auto sink = dynamic_cast<rshp::base::AbstractSink*>(node); sink != nullptr)
		for(auto inputDataPort : sink->getInputDataPorts())
			inputDataPorts.push_back(std::make_unique<InputDataPort>(inputDataPort));

	if(auto source = dynamic_cast<rshp::base::AbstractSource*>(node); source != nullptr)
		for(auto OutputPort : source->getOutputDataPorts())
			outputDataPorts.push_back(std::make_unique<OutputDataPort>(OutputPort));
	node->registerInputEvents();
	node->registerOutputEvents();
	for(auto& [index, port] : node->getInputEventPorts())
		inputEventPorts.push_back(std::make_unique<InputEventPort>(port.get()));

	for(auto& [index, port] : node->getOutputEventPorts())
		outputEventPorts.push_back(std::make_unique<OutputEventPort>(&port));
}

bool Node::hasInputs() const
{
	return !inputDataPorts.empty() || !inputEventPorts.empty();
}

bool Node::hasOutputs() const
{
	return !outputDataPorts.empty() || !outputEventPorts.empty();
}

void Node::initializeLayout()
{
	float const itemSpacing = ImGui::GetStyle().ItemSpacing.x;
	float const minimumCenterSpacing = 20;

	auto measurePinGroup = [&](auto& dataPins, auto& eventPins) {
		float pinGroupWidth = 0;
		for(auto const& pin : dataPins)
			pinGroupWidth = std::max(pinGroupWidth, pin->calculateSize().x);
		for(auto const& pin : eventPins)
			pinGroupWidth = std::max(pinGroupWidth, pin->calculateSize().x);

		if(dataPins.size() > 0 || eventPins.size() > 0)
			contentsWidth += pinGroupWidth + itemSpacing * 2;
		return pinGroupWidth;
	};

	measurePinGroup(inputDataPorts, inputEventPorts);
	outputsWidth = measurePinGroup(outputDataPorts, outputEventPorts);

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
}

void Node::drawInputs()
{
	if(hasInputs())
	{
		ImGui::BeginGroup();
		for(auto& InputPort : inputDataPorts)
		{
			InputPort->draw();
			ImGui::SameLine();
			ImGui::Dummy({0, 0});
		}
		for(auto& InputPort : inputEventPorts)
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
		for(auto& OutputPort : outputDataPorts)
		{
			ImGui::Dummy({outputsWidth - OutputPort->calculateSize().x, 0});
			ImGui::SameLine();
			OutputPort->draw();
		}
		for(auto& OutputPort : outputEventPorts)
		{
			ImGui::Dummy({outputsWidth - OutputPort->calculateSize().x, 0});
			ImGui::SameLine();
			OutputPort->draw();
		}
		ImGui::EndGroup();
	}
}

void Node::draw()
{
	if(!layoutInitialized)
		initializeLayout();
	auto& nodeEditorStyle = ax::NodeEditor::GetStyle();

	nodeEditorStyle.NodeBorderWidth = Stylesheet::getCurrentSheet().nodeBorderWidth;

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
	for(auto& InputPort : inputDataPorts)
		InputPort->drawLink();
	for(auto& InputEventPort : inputEventPorts)
		InputEventPort->drawLink();
}

} // namespace rshp::gui