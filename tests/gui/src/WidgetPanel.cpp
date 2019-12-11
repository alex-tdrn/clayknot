#include "WidgetPanel.h"
#include "rsp/gui/widgets/Editor.hpp"
#include "rsp/gui/widgets/Viewer.hpp"

WidgetPanel::WidgetPanel()
{
	title = "WidgetPanel";

	auto createWidgets = [&](std::vector<rsp::gui::Widget*>& widgetGroup, auto& resource, std::string prefix) {
		allWidgets.push_back(rsp::gui::makeViewer(resource, prefix + " Viewer"));
		widgetGroup.push_back(allWidgets.back().get());
		widgetGroup.back()->preferCompact();

		allWidgets.push_back(rsp::gui::makeEditor(resource, prefix + " Editor"));
		widgetGroup.push_back(allWidgets.back().get());
		widgetGroup.back()->preferCompact();
	};

	createWidgets(booleanWidgets, booleanVar, "Boolean");

	createWidgets(integerWidgets, integerVar, "Integer");
	createWidgets(integerWidgets, boundedIntegerVar, "Bounded Integer");

	createWidgets(floatWidgets, floatVar, "Float");
	createWidgets(floatWidgets, boundedFloatVar, "Bounded Float");

	createWidgets(vector2Widgets, vector2Var, "Vector2");
	createWidgets(vector2Widgets, boundedVector2Var, "Bounded Vector2");

	createWidgets(vector3Widgets, vector3Var, "Vector3");
	createWidgets(vector3Widgets, boundedVector3Var, "Bounded Vector3");

	createWidgets(vector4Widgets, vector4Var, "Vector4");
	createWidgets(vector4Widgets, boundedVector4Var, "Bounded Vector4");

	createWidgets(colorRGBWidgets, colorRGBVar, "ColorRGB");

	createWidgets(colorRGBAWidgets, colorRGBAVar, "ColorRGBA");

	createWidgets(durationWidgets, durationVar, "Duration");
}

void WidgetPanel::drawWidgetsGroup(std::vector<rsp::gui::Widget*>& widgetsGroup)
{
	for(auto& widget : widgetsGroup)
	{
		widget->draw();
		if(drawWidgetBorders)
		{
			ImGui::GetWindowDrawList()->AddRect(
				ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImGui::GetColorU32({1.0f, 1.0f, 1.0f, 0.25f}));
		}
		ImGui::NewLine();
	}
}

void WidgetPanel::drawContents()
{
	ImGui::Checkbox("Draw Widget Borders", &drawWidgetBorders);
	if(ImGui::DragFloat("Max Widgets Width", &maxWidgetWidth, 1.0f, 0.0f, 10000.0f))
	{
		if(maxWidgetWidth == 0.0f)
		{
			for(auto& widget : allWidgets)
				widget->clearMaximumWidth();
		}
		else
		{
			for(auto& widget : allWidgets)
				widget->setMaximumWidth(maxWidgetWidth);
		}
	}

	if(ImGui::CollapsingHeader("Boolean"))
		drawWidgetsGroup(booleanWidgets);

	if(ImGui::CollapsingHeader("Integer"))
		drawWidgetsGroup(integerWidgets);

	if(ImGui::CollapsingHeader("Float"))
		drawWidgetsGroup(floatWidgets);

	if(ImGui::CollapsingHeader("Vector2"))
		drawWidgetsGroup(vector2Widgets);

	if(ImGui::CollapsingHeader("Vector3"))
		drawWidgetsGroup(vector3Widgets);

	if(ImGui::CollapsingHeader("Vector4"))
		drawWidgetsGroup(vector4Widgets);

	if(ImGui::CollapsingHeader("ColorRGB"))
		drawWidgetsGroup(colorRGBWidgets);

	if(ImGui::CollapsingHeader("ColorRGBA"))
		drawWidgetsGroup(colorRGBAWidgets);

	if(ImGui::CollapsingHeader("Duration"))
		drawWidgetsGroup(durationWidgets);
}