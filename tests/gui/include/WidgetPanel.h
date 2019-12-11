#pragma once

#include "rsp/base/Bounded.hpp"
#include "rsp/base/ColorRGB.hpp"
#include "rsp/base/ColorRGBA.hpp"
#include "rsp/gui/panels/Panel.h"
#include "rsp/gui/widgets/Widget.hpp"

#include <chrono>
#include <memory>
#include <vector>

class WidgetPanel : public rsp::gui::Panel
{
private:
	bool drawWidgetBorders = true;
	float maxWidgetWidth = 0;

	bool booleanVar{true};

	int integerVar{0};
	rsp::Bounded<int> boundedIntegerVar{75, 0, 100};

	float floatVar{0.0f};
	rsp::Bounded<float> boundedFloatVar{0.1f, 0.0f, 1.0f};

	glm::vec2 vector2Var{1.0f, 2.0f};
	rsp::Bounded<glm::vec2> boundedVector2Var{glm::vec2{0.0f}, glm::vec2{-180.0f, -90.0f}, glm::vec2{180.0f, 90.0f}};

	glm::vec3 vector3Var{1.0f, 2.0f, 3.0f};
	rsp::Bounded<glm::vec3> boundedVector3Var{
		glm::vec3{0.0f}, glm::vec3{-180.0f, -90.0f, -180.0f}, glm::vec3{180.0f, 90.0f, 180.0f}};

	glm::vec4 vector4Var{1.0f, 2.0f, 3.0f, 1.0f};
	rsp::Bounded<glm::vec4> boundedVector4Var{glm::vec4{1.0f, 2.0f, 3.0f, 1.0f},
		glm::vec4{-1000.0f, -1000.0f, -1000.0f, 0.0f}, glm::vec4{1000.0f, 1000.0f, 1000.0f, 1.0f}};

	rsp::ColorRGB colorRGBVar{0.5f, 0.75f, 0.25f};

	rsp::ColorRGBA colorRGBAVar{0.5f, 0.75f, 0.25f, 0.5f};

	std::chrono::nanoseconds durationVar{123467890};

	std::vector<std::unique_ptr<rsp::gui::Widget>> allWidgets;
	std::vector<rsp::gui::Widget*> booleanWidgets;
	std::vector<rsp::gui::Widget*> integerWidgets;
	std::vector<rsp::gui::Widget*> floatWidgets;
	std::vector<rsp::gui::Widget*> vector2Widgets;
	std::vector<rsp::gui::Widget*> vector3Widgets;
	std::vector<rsp::gui::Widget*> vector4Widgets;
	std::vector<rsp::gui::Widget*> colorRGBWidgets;
	std::vector<rsp::gui::Widget*> colorRGBAWidgets;
	std::vector<rsp::gui::Widget*> durationWidgets;

public:
	WidgetPanel();
	~WidgetPanel() = default;

private:
	void drawWidgetsGroup(std::vector<rsp::gui::Widget*>& widgetsGroup);

protected:
	void drawContents() override;
};