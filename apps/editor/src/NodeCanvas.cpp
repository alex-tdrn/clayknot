#include "NodeCanvas.h"
#include "AbstractPin.hpp"
#include "rshp/base/AbstractPipe.hpp"
#include "Stylesheet.hpp"

NodeCanvas::NodeCanvas()
{
	context = ax::NodeEditor::CreateEditor();
	title = "Pipeline Canvas " + std::to_string(id);
	windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
}

NodeCanvas::~NodeCanvas()
{
	ax::NodeEditor::DestroyEditor(context);
}

void NodeCanvas::drawContents()
{
	if(ImGui::Begin(title.c_str(), &visible, windowFlags))
	{
		ax::NodeEditor::SetCurrentEditor(context);
		ax::NodeEditor::Begin(title.c_str());
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

		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_Bg] = Stylesheet::getCurrentSheet().nodeCanvasBackgroundColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_Grid] = Stylesheet::getCurrentSheet().nodeCanvasGridColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeBg] = Stylesheet::getCurrentSheet().nodeBackgroundColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeBorder] = Stylesheet::getCurrentSheet().nodeBorderColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_HovNodeBorder] = Stylesheet::getCurrentSheet().hoveredNodeBorderColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_SelNodeBorder] = Stylesheet::getCurrentSheet().selectedNodeBorderColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeSelRect] = Stylesheet::getCurrentSheet().nodeSelectionRectColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeSelRectBorder] = Stylesheet::getCurrentSheet().nodeSelectionRectBorderColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_HovLinkBorder] = Stylesheet::getCurrentSheet().hoveredLinkBorderColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_SelLinkBorder] = Stylesheet::getCurrentSheet().selectedLinkBorderColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_LinkSelRect] = Stylesheet::getCurrentSheet().linkSelectionRectColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_LinkSelRectBorder] = Stylesheet::getCurrentSheet().linkSelectionRectBorderColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_PinRect] = Stylesheet::getCurrentSheet().pinRectColor;
		ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_PinRectBorder] = Stylesheet::getCurrentSheet().pinRectBorderColor;

		for(auto& node : nodes)
			node.draw();
		for(auto& node : nodes)
			node.drawInputLinks();
		if(ax::NodeEditor::BeginCreate())
		{
			ax::NodeEditor::PinId idPin1 = 0, idPin2 = 0;
			if(ax::NodeEditor::QueryNewLink(&idPin1, &idPin2) && idPin1 != idPin2)
			{
				auto pin1 = AbstractPin::getPinForID(idPin1);
				auto pin2 = AbstractPin::getPinForID(idPin2);
				if(pin1->canConnect(pin2))
				{
					if(ax::NodeEditor::AcceptNewItem({0, 1, 0, 1, }, 2))
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
			for(auto [name, constructor] : AbstractPipe::getPipeMap())
			{
				if(ImGui::MenuItem(name.c_str()))
				{
					auto source = constructor();
					nodes.emplace_back(source.get());
					store->push_back(std::move(source));
				}
			}
			ImGui::EndPopup();
		}
		ax::NodeEditor::Resume();


		ax::NodeEditor::End();
	}
	ImGui::End();
}

void NodeCanvas::setStore(std::vector<std::unique_ptr<AbstractPipe>>* store)
{
	this->store = store;
	for(auto& pipe : *store)
		nodes.emplace_back(pipe.get());
}
