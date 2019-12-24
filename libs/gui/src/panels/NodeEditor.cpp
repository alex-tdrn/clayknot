#include "rsp/gui/panels/NodeEditor.h"
#include "rsp/base/node/AbstractNode.hpp"
#include "rsp/base/node/FixedSource.hpp"
#include "rsp/gui/Stylesheet.hpp"
#include "rsp/gui/nodes/AbstractPort.hpp"
#include "rsp/gui/widgets/Editor.hpp"

namespace rsp::gui
{
NodeEditor::NodeEditor()
{
	SupportedEditorTypes::for_each([&](auto* t) {
		using ResourceType = std::remove_reference_t<decltype(*t)>;
		rsp::FixedSource<ResourceType> node;
	});

	context = ax::NodeEditor::CreateEditor();
	title = "Graph Canvas " + std::to_string(id);
	flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
}

NodeEditor::~NodeEditor()
{
	ax::NodeEditor::DestroyEditor(context);
}

void NodeEditor::toggleInputWidgets()
{
	int visibleCount = 0;
	int hiddenCount = 0;
	for(auto& node : nodes)
	{
		visibleCount += node->countVisibleInputWidgets();
		hiddenCount += node->countHiddenInputWidgets();
	}
	bool visible = visibleCount < hiddenCount;
	showInputWidgets = visible;
	for(auto& node : nodes)
		node->setInputWidgetsVisibility(visible);
}

void NodeEditor::toggleOutputWidgets()
{
	int visibleCount = 0;
	int hiddenCount = 0;
	for(auto& node : nodes)
	{
		visibleCount += node->countVisibleOutputWidgets();
		hiddenCount += node->countHiddenOutputWidgets();
	}
	bool visible = visibleCount < hiddenCount;
	showOutputWidgets = visible;
	for(auto& node : nodes)
		node->setOutputWidgetsVisibility(visible);
}

void NodeEditor::toggleAllWidgets()
{
	int visibleCount = 0;
	int hiddenCount = 0;
	for(auto& node : nodes)
	{
		visibleCount += node->countVisibleInputWidgets();
		visibleCount += node->countVisibleOutputWidgets();
		hiddenCount += node->countHiddenInputWidgets();
		hiddenCount += node->countHiddenOutputWidgets();
	}
	bool visible = visibleCount < hiddenCount;
	showInputWidgets = visible;
	showOutputWidgets = visible;
	for(auto& node : nodes)
	{
		node->setInputWidgetsVisibility(visible);
		node->setOutputWidgetsVisibility(visible);
	}
}

void NodeEditor::drawContents()
{
	glm::vec2 overlayPosition = {10.0f, 30.0f};

	ax::NodeEditor::SetCurrentEditor(context);
	ax::NodeEditor::Begin(title.c_str());

	overlayPosition += glm::vec2(ImGui::GetWindowPos());

	ax::NodeEditor::GetStyle().LinkStrength = Stylesheet::getCurrentSheet().linkStrength;
	ax::NodeEditor::GetStyle().NodePadding = Stylesheet::getCurrentSheet().nodePadding;
	ax::NodeEditor::GetStyle().NodeRounding = Stylesheet::getCurrentSheet().nodeRounding;
	ax::NodeEditor::GetStyle().NodeBorderWidth = Stylesheet::getCurrentSheet().nodeBorderWidth;
	ax::NodeEditor::GetStyle().HoveredNodeBorderWidth = Stylesheet::getCurrentSheet().hoveredNodeBorderWidth;
	ax::NodeEditor::GetStyle().SelectedNodeBorderWidth = Stylesheet::getCurrentSheet().selectedNodeBorderWidth;
	ax::NodeEditor::GetStyle().PinRounding = Stylesheet::getCurrentSheet().pinRounding;
	ax::NodeEditor::GetStyle().PinBorderWidth = Stylesheet::getCurrentSheet().pinBorderWidth;
	ax::NodeEditor::GetStyle().SourceDirection = Stylesheet::getCurrentSheet().sourceDirection;
	ax::NodeEditor::GetStyle().TargetDirection = Stylesheet::getCurrentSheet().targetDirection;
	ax::NodeEditor::GetStyle().FlowMarkerDistance = Stylesheet::getCurrentSheet().flowMarkerDistance;
	ax::NodeEditor::GetStyle().FlowSpeed = Stylesheet::getCurrentSheet().flowSpeed;
	ax::NodeEditor::GetStyle().FlowDuration = Stylesheet::getCurrentSheet().flowDuration;
	ax::NodeEditor::GetStyle().PivotAlignment = Stylesheet::getCurrentSheet().pivotAlignment;
	ax::NodeEditor::GetStyle().PivotSize = Stylesheet::getCurrentSheet().pivotSize;
	ax::NodeEditor::GetStyle().PivotScale = Stylesheet::getCurrentSheet().pivotScale;
	ax::NodeEditor::GetStyle().PinRadius = Stylesheet::getCurrentSheet().pinRadius;
	ax::NodeEditor::GetStyle().PinArrowSize = Stylesheet::getCurrentSheet().pinArrowSize;
	ax::NodeEditor::GetStyle().PinArrowWidth = Stylesheet::getCurrentSheet().pinArrowWidth;

	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_Bg] =
		Stylesheet::getCurrentSheet().nodeEditorBackgroundColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_Grid] =
		Stylesheet::getCurrentSheet().nodeEditorGridColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeBg] =
		Stylesheet::getCurrentSheet().nodeBackgroundColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeBorder] =
		Stylesheet::getCurrentSheet().nodeBorderColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_HovNodeBorder] =
		Stylesheet::getCurrentSheet().hoveredNodeBorderColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_SelNodeBorder] =
		Stylesheet::getCurrentSheet().selectedNodeBorderColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeSelRect] =
		Stylesheet::getCurrentSheet().nodeSelectionRectColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeSelRectBorder] =
		Stylesheet::getCurrentSheet().nodeSelectionRectBorderColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_HovLinkBorder] =
		Stylesheet::getCurrentSheet().hoveredLinkBorderColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_SelLinkBorder] =
		Stylesheet::getCurrentSheet().selectedLinkBorderColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_LinkSelRect] =
		Stylesheet::getCurrentSheet().linkSelectionRectColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_LinkSelRectBorder] =
		Stylesheet::getCurrentSheet().linkSelectionRectBorderColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_PinRect] = Stylesheet::getCurrentSheet().pinRectColor;
	ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_PinRectBorder] =
		Stylesheet::getCurrentSheet().pinRectBorderColor;

	for(auto& node : nodes)
		node->draw();
	for(auto& node : nodes)
		node->drawInputLinks();
	if(ax::NodeEditor::BeginCreate())
	{
		ax::NodeEditor::PinId idPin1 = 0, idPin2 = 0;
		if(ax::NodeEditor::QueryNewLink(&idPin1, &idPin2) && idPin1 != idPin2)
		{
			auto pin1 = AbstractPort::getPinForID(idPin1);
			auto pin2 = AbstractPort::getPinForID(idPin2);
			if(pin1->canConnect(pin2))
			{
				if(ax::NodeEditor::AcceptNewItem(
					   {
						   0,
						   1,
						   0,
						   1,
					   },
					   2))
				{
					pin1->connect(pin2);
				}
			}
			else
			{
				ax::NodeEditor::RejectNewItem({1, 0, 0, 1}, 2);
			}
		}

		ax::NodeEditor::PinId pinId = 0;
		if(ax::NodeEditor::QueryNewNode(&pinId))
		{
			if(ax::NodeEditor::AcceptNewItem())
			{
				ax::NodeEditor::Suspend();
				ImGui::OpenPopup("Create New Node");
				ax::NodeEditor::Resume();
			}
		}
	}
	ax::NodeEditor::EndCreate();

	ax::NodeEditor::Suspend();
	if(ImGui::BeginPopup("<<Create New Link>>"))
	{
		ImGui::Text("<<< TEST >>>");
		ImGui::Text("Create New Link");

		ImGui::EndPopup();
	}

	if(ImGui::BeginPopup("Create New Node"))
	{
		for(auto [name, constructor] : rsp::AbstractNode::getNodeMap())
		{
			if(ImGui::MenuItem(name.c_str()))
			{
				auto source = constructor();
				nodes.push_back(std::make_unique<Node>(source.get()));
				nodes.back()->setInputWidgetsVisibility(showInputWidgets);
				nodes.back()->setOutputWidgetsVisibility(showOutputWidgets);
				store->push_back(std::move(source));
			}
		}
		ImGui::EndPopup();
	}
	ax::NodeEditor::Resume();

	ax::NodeEditor::End();
	ImGui::SetNextWindowPos(overlayPosition);
	ImGui::PushID(this);
	if(ImGui::Begin("Node Editor Overlay", nullptr,
		   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
			   ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if(ImGui::Button("Toggle input widgets"))
			toggleInputWidgets();
		if(ImGui::Button("Toggle output widgets"))
			toggleOutputWidgets();
		if(ImGui::Button("Toggle all widgets"))
		{
			toggleInputWidgets();
			toggleOutputWidgets();
		}
	}
	ImGui::End();
	ImGui::PopID();
}

void NodeEditor::setStore(std::vector<std::unique_ptr<rsp::AbstractNode>>* store)
{
	this->store = store;
	for(auto& node : *store)
		nodes.push_back(std::make_unique<Node>(node.get()));
}

} // namespace rsp::gui