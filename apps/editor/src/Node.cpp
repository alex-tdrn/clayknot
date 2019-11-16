#include "Node.h"
#include "UniqueID.hpp"
#include "UIDebug.hpp"
#include "rshp/base/AbstractSink.hpp"
#include "rshp/base/AbstractSource.hpp"
#include "Stylesheet.hpp"

#include <algorithm>

Node::Node(AbstractPipe* pipe)
	: pipe(pipe), id(uniqueID())
{
	if(auto sink = dynamic_cast<AbstractSink*>(pipe); sink != nullptr)
		for(auto inputDataPort : sink->getInputDataPorts())
			inputDataPins.push_back(std::make_unique<InputDataPin>(inputDataPort));
	
	if(auto source = dynamic_cast<AbstractSource*>(pipe); source != nullptr)
		for(auto outputPin : source->getOutputDataPorts())
			outputDataPins.push_back(std::make_unique<OutputDataPin>(outputPin));
	pipe->registerInputEvents();
	pipe->registerOutputEvents();
	for(auto& [index, port] : pipe->getInputEventPorts())
		inputEventPins.push_back(std::make_unique<InputEventPin>(port.get()));

	for(auto& [index, port] : pipe->getOutputEventPorts())
		outputEventPins.push_back(std::make_unique<OutputEventPin>(&port));
}

bool Node::hasInputs() const
{
	return !inputDataPins.empty() || !inputEventPins.empty();
}

bool Node::hasOutputs() const
{
	return !outputDataPins.empty() || !outputEventPins.empty();
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

	measurePinGroup(inputDataPins, inputEventPins);
	outputsWidth = measurePinGroup(outputDataPins, outputEventPins);

	float const titleWidth = ImGui::CalcTextSize(pipe->getName().c_str()).x;

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

	ImGui::Text(pipe->getName().c_str());

	if(titleOffset > 0)
		ImGui::Unindent(titleOffset);
}

void Node::drawInputs()
{
	if(hasInputs())
	{
		ImGui::BeginGroup();
		for(auto& inputPin : inputDataPins)
		{
			inputPin->draw();
			ImGui::SameLine();
			ImGui::Dummy({0, 0});
		}
		for(auto& inputPin : inputEventPins)
		{
			inputPin->draw();
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
		for(auto& outputPin : outputDataPins)
		{
			ImGui::Dummy({outputsWidth - outputPin->calculateSize().x, 0});
			ImGui::SameLine();
			outputPin->draw();
		}
		for(auto& outputPin : outputEventPins)
		{
			ImGui::Dummy({outputsWidth - outputPin->calculateSize().x, 0});
			ImGui::SameLine();
			outputPin->draw();
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
	for(auto& inputPin : inputDataPins)
		inputPin->drawLink();
	for(auto& inputEventPin : inputEventPins)
		inputEventPin->drawLink();
}

