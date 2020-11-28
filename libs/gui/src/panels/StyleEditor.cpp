#include "rsp/gui/panels/StyleEditor.h"
#include "rsp/gui/widgets/Editor.hpp"

#include <imgui.h>
#include <string>

namespace rsp::gui
{
StyleEditor::StyleEditor()
{
	setStylesheet(Stylesheet::getCurrentSheet());
}

void StyleEditor::setStylesheet(Stylesheet& sheet)
{
	this->sheet = &sheet;
	title = "Stylesheet " + sheet.getName();

	styleWidgets.clear();

	styleWidgets.push_back(makeEditor(this->sheet->nodeEditorBackgroundColor, "Canvas Background Color"));
	styleWidgets.push_back(makeEditor(this->sheet->nodeEditorGridColor, "Canvas Grid Color"));
	styleWidgets.push_back(makeEditor(this->sheet->nodeSelectionRectColor, "Node Selection Rect Color"));
	styleWidgets.push_back(makeEditor(this->sheet->nodeSelectionRectBorderColor, "Node Selection Rect Border Color"));
	styleWidgets.push_back(makeEditor(this->sheet->linkSelectionRectColor, "Link Selection Rect Color"));
	styleWidgets.push_back(makeEditor(this->sheet->linkSelectionRectBorderColor, "Link Selection Rect Border Color"));
	styleWidgets.push_back(makeEditor(this->sheet->nodeBackgroundColor, "Node Background Color"));
	styleWidgets.push_back(makeEditor(this->sheet->nodeBorderColor, "Node Border Color"));
	styleWidgets.push_back(makeEditor(this->sheet->hoveredNodeBorderColor, "Hovered Node Border Color"));
	styleWidgets.push_back(makeEditor(this->sheet->selectedNodeBorderColor, "Selected Node Border Color"));
	styleWidgets.push_back(makeEditor(this->sheet->nodeBorderWidth, "Node Border Width"));
	styleWidgets.push_back(makeEditor(this->sheet->hoveredNodeBorderWidth, "Hovered Border Width"));
	styleWidgets.push_back(makeEditor(this->sheet->selectedNodeBorderWidth, "Selected Border Width"));
	styleWidgets.push_back(makeEditor(this->sheet->nodeRounding, "Node Rounding"));
	styleWidgets.push_back(makeEditor(this->sheet->nodePadding, "Node Padding"));
	styleWidgets.push_back(makeEditor(this->sheet->pinRectColor, "Pin Rect Color"));
	styleWidgets.push_back(makeEditor(this->sheet->pinRectBorderColor, "Pin Rect Border Color"));
	styleWidgets.push_back(makeEditor(this->sheet->eventColor, "Event Color"));
	styleWidgets.push_back(makeEditor(this->sheet->eventTextColor, "Event Text Color"));
	styleWidgets.push_back(makeEditor(this->sheet->anchorOffset, "Anchor Offset"));
	styleWidgets.push_back(makeEditor(this->sheet->animatedAnchorOffset, "Animated Anchor Offset"));
	styleWidgets.push_back(makeEditor(this->sheet->animatedAnchorOffsetDuration, "Duration Anchor Offset Animation"));
	styleWidgets.push_back(makeEditor(this->sheet->pinBorderWidth, "Pin Border Width"));
	styleWidgets.push_back(makeEditor(this->sheet->pinRounding, "Pin Rounding"));
	styleWidgets.push_back(makeEditor(this->sheet->pinRadius, "Pin Radius"));
	styleWidgets.push_back(makeEditor(this->sheet->pinArrowSize, "Pin Arrow Size"));
	styleWidgets.push_back(makeEditor(this->sheet->pinArrowWidth, "Pin Arrow Width"));
	styleWidgets.push_back(makeEditor(this->sheet->hoveredLinkBorderColor, "Hovered Link Border Color"));
	styleWidgets.push_back(makeEditor(this->sheet->selectedLinkBorderColor, "Selected Link Border Color"));
	styleWidgets.push_back(makeEditor(this->sheet->linkStrength, "Link Strength"));
	styleWidgets.push_back(makeEditor(this->sheet->linkThickness, "Link Thickness"));
	styleWidgets.push_back(makeEditor(this->sheet->targetDirection, "Target Direction"));
	styleWidgets.push_back(makeEditor(this->sheet->sourceDirection, "Source Direction"));
	styleWidgets.push_back(makeEditor(this->sheet->flowMarkerDistance, "Flow Marker Distance"));
	styleWidgets.push_back(makeEditor(this->sheet->flowSpeed, "Flow Speed"));
	styleWidgets.push_back(makeEditor(this->sheet->flowDuration, "Flow Duration"));
}

void StyleEditor::drawContents()
{
	for(auto& widget : styleWidgets)
	{
		widget->draw();
		ImGui::NewLine();
	}
}

} // namespace rsp::gui