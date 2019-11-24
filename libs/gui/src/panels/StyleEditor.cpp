#include "rshp/gui/panels/StyleEditor.h"

namespace rshp::gui
{
StyleEditor::StyleEditor()
{
	setStylesheet(Stylesheet::getCurrentSheet());
}

void StyleEditor::setStylesheet(Stylesheet& sheet)
{
	this->sheet = &sheet;
	title = "Stylesheet " + sheet.getName();
}

void StyleEditor::drawContents()
{
	if(ImGui::Begin(title.c_str(), &visible, flags))
	{
		if(ImGui::CollapsingHeader("Canvas", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Canvas Background Color");
			ImGui::ColorEdit4("##CanvasBackgroundColor", &sheet->nodeEditorBackgroundColor.x);
			ImGui::NewLine();

			ImGui::Text("Canvas Grid Color");
			ImGui::ColorEdit4("##CanvasGridColor", &sheet->nodeEditorGridColor.x);
			ImGui::NewLine();

			ImGui::Text("Node Selection Rect Color");
			ImGui::ColorEdit4("##NodeSelectionRectColor", &sheet->nodeSelectionRectColor.x);
			ImGui::NewLine();

			ImGui::Text("Node Selection Rect Border Color");
			ImGui::ColorEdit4("##NodeSelectionRectBorderColor", &sheet->nodeSelectionRectBorderColor.x);
			ImGui::NewLine();

			ImGui::Text("Link Selection Rect Color");
			ImGui::ColorEdit4("##LinkSelectionRectColor", &sheet->linkSelectionRectColor.x);
			ImGui::NewLine();

			ImGui::Text("Link Selection Rect Border Color");
			ImGui::ColorEdit4("##LinkSelectionRectBorderColor", &sheet->linkSelectionRectBorderColor.x);
			ImGui::NewLine();
		}

		if(ImGui::CollapsingHeader("Nodes", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Node Background Color");
			ImGui::ColorEdit4("##NodeBackgroundColor", &sheet->nodeBackgroundColor.x);
			ImGui::NewLine();

			ImGui::Text("Node Border Color");
			ImGui::ColorEdit4("##NodeBorderColor", &sheet->nodeBorderColor.x);
			ImGui::NewLine();

			ImGui::Text("Hovered Node Border Color");
			ImGui::ColorEdit4("##HoveredNodeBorderColor", &sheet->hoveredNodeBorderColor.x);
			ImGui::NewLine();

			ImGui::Text("Selected Node Border Color");
			ImGui::ColorEdit4("##SelectedNodeBorderColor", &sheet->selectedNodeBorderColor.x);
			ImGui::NewLine();

			ImGui::Text("Node Border Width");
			ImGui::SliderFloat("##NodeBorderWidth", &sheet->nodeBorderWidth, 1.0f, 10.0f);
			ImGui::NewLine();

			ImGui::Text("Hovered Border Width");
			ImGui::SliderFloat("##HoveredBorderWidth", &sheet->hoveredNodeBorderWidth, 1.0f, 10.0f);
			ImGui::NewLine();

			ImGui::Text("Selected Border Width");
			ImGui::SliderFloat("##SelectedBorderWidth", &sheet->selectedNodeBorderWidth, 1.0f, 10.0f);
			ImGui::NewLine();

			ImGui::Text("Node Rounding");
			ImGui::SliderFloat("##NodeRounding", &sheet->nodeRounding, 0.0f, 100.0f);
			ImGui::NewLine();

			ImGui::Text("Node Padding");
			ImGui::SliderFloat4("##NodePadding", &sheet->nodePadding.x, 0.0f, 100.0f);
			ImGui::NewLine();
		}

		if(ImGui::CollapsingHeader("Pins", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Pin Rect Color");
			ImGui::ColorEdit4("##PinRectColor", &sheet->pinRectColor.x);
			ImGui::NewLine();

			ImGui::Text("Pin Rect Border Color");
			ImGui::ColorEdit4("##PinRectBorderColor", &sheet->pinRectBorderColor.x);
			ImGui::NewLine();

			ImGui::Text("Event Color");
			ImGui::ColorEdit4("##EventColor", &sheet->eventColor.x);
			ImGui::NewLine();

			ImGui::Text("Event Text Color");
			ImGui::ColorEdit4("##EventTextColor", &sheet->eventTextColor.x);
			ImGui::NewLine();

			ImGui::Text("Anchor Offset");
			ImGui::SliderFloat("##AnchorOffset", &sheet->anchorOffset, 0.0f, 100.0f);
			ImGui::NewLine();

			ImGui::Text("Animated Anchor Offset");
			ImGui::SliderFloat("##AnimatedAnchorOffset", &sheet->animatedAnchorOffset, 0.0f, 100.0f);
			ImGui::NewLine();

			ImGui::Text("Duration Anchor Offset Animation");
			int duration = sheet->animatedAnchorOffsetDuration.count();
			ImGui::InputInt("##DurationAnchorOffsetAnimation", &duration, 10, 100);
			if(duration < 0)
				duration = 0;
			sheet->animatedAnchorOffsetDuration = std::chrono::milliseconds(duration);
			ImGui::NewLine();

			ImGui::Text("Pin Border Width");
			ImGui::SliderFloat("##PinBorderWidth", &sheet->pinBorderWidth, 0.0f, 100.0f);
			ImGui::NewLine();

			ImGui::Text("Pin Rounding");
			ImGui::SliderFloat("##PinRounding", &sheet->pinRounding, 0.0f, 100.0f);
			ImGui::NewLine();

			ImGui::Text("Pin Radius");
			ImGui::SliderFloat("##PinRadius", &sheet->pinRadius, 0.0f, 100.0f);
			ImGui::NewLine();

			ImGui::Text("Pin Arrow Size");
			ImGui::SliderFloat("##PinArrowSize", &sheet->pinArrowSize, 0.0f, 100.0f);
			ImGui::NewLine();

			ImGui::Text("Pin Arrow Width");
			ImGui::SliderFloat("##PinArrowWidth", &sheet->pinArrowWidth, 0.0f, 100.0f);
			ImGui::NewLine();
		}

		if(ImGui::CollapsingHeader("Links", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Hovered Link Border Color");
			ImGui::ColorEdit4("##HoveredLinkBorderColor", &sheet->hoveredLinkBorderColor.x);
			ImGui::NewLine();

			ImGui::Text("Selected Link Border Color");
			ImGui::ColorEdit4("##SelectedLinkBorderColor", &sheet->selectedLinkBorderColor.x);
			ImGui::NewLine();

			ImGui::Text("Link Strength");
			ImGui::SliderFloat("##LinkStrength", &sheet->linkStrength, 0.0f, 1000.0f);
			ImGui::NewLine();

			ImGui::Text("Link Thickness");
			ImGui::SliderFloat("##LinkThickness", &sheet->linkThickness, 1.0f, 10.0f);
			ImGui::NewLine();

			ImGui::Text("Target Direction");
			ImGui::SliderFloat2("##TargetDirection", &sheet->targetDirection.x, -1.0f, 1.0f);
			ImGui::NewLine();

			ImGui::Text("Source Direction");
			ImGui::SliderFloat2("##SourceDirection", &sheet->sourceDirection.x, -1.0f, 1.0f);
			ImGui::NewLine();

			ImGui::Text("Flow Marker Distance");
			ImGui::SliderFloat("##FlowMarkerDistance", &sheet->flowMarkerDistance, 0.0f, 100.0f);
			ImGui::NewLine();

			ImGui::Text("Flow Speed");
			ImGui::SliderFloat("##FlowSpeed", &sheet->flowSpeed, 0.0f, 1000.0f);
			ImGui::NewLine();

			ImGui::Text("Flow Duration");
			ImGui::SliderFloat("##FlowDuration", &sheet->flowDuration, 0.0f, 10.0f);
			ImGui::NewLine();
		}
	}
	ImGui::End();
}

} // namespace rshp::gui