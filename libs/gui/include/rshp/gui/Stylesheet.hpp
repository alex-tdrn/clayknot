#pragma once

#include "rshp/gui/Animation.hpp"

#include <chrono>
#include <imgui.h>
#include <string>
#include <unordered_map>

using namespace std::chrono_literals;

namespace rshp::gui
{
class Stylesheet
{
private:
	static inline std::unordered_map<std::string, Stylesheet> sheets;
	static inline std::string currentSheetName;
	static inline Stylesheet* currentSheet = nullptr;
	std::string name = "default";

public:
	ImVec4 nodeEditorBackgroundColor = ImColor(60, 60, 70, 200);
	ImVec4 nodeEditorGridColor = ImColor(120, 120, 120, 40);
	ImVec4 nodeSelectionRectColor = ImColor(5, 130, 255, 64);
	ImVec4 nodeSelectionRectBorderColor = ImColor(5, 130, 255, 128);
	ImVec4 linkSelectionRectColor = ImColor(5, 130, 255, 64);
	ImVec4 linkSelectionRectBorderColor = ImColor(5, 130, 255, 128);

	ImVec4 nodeBackgroundColor = ImColor(32, 32, 32, 200);
	ImVec4 nodeBorderColor = ImColor(255, 255, 255, 96);
	ImVec4 hoveredNodeBorderColor = ImColor(50, 176, 255, 255);
	ImVec4 selectedNodeBorderColor = ImColor(255, 176, 50, 255);
	float nodeBorderWidth = 2;
	float hoveredNodeBorderWidth = 3.5f;
	float selectedNodeBorderWidth = 3.5f;
	float nodeRounding = 0.0f;
	ImVec4 nodePadding = {8, 8, 8, 8};

	ImVec4 pinRectColor = ImColor(60, 180, 255, 100);
	ImVec4 pinRectBorderColor = ImColor(60, 180, 255, 128);
	ImVec4 eventColor = {1, 1, 1, 1};
	ImVec4 eventTextColor = {1, 0.9, 0.68, 1};
	float anchorOffset = 7;
	float animatedAnchorOffset = 10;
	std::chrono::milliseconds animatedAnchorOffsetDuration = 1s;
	float pinBorderWidth = 0.0f;
	float pinRounding = 0.0f;
	float pinRadius = 0.0f;
	float pinArrowSize = 0.0f;
	float pinArrowWidth = 0.0f;

	ImVec4 hoveredLinkBorderColor = ImColor(50, 176, 255, 255);
	ImVec4 selectedLinkBorderColor = ImColor(255, 176, 50, 255);
	float linkStrength = 1000;
	float linkThickness = 2;
	ImVec2 targetDirection = {-1.0f, 0.0f};
	ImVec2 sourceDirection = {1.0f, 0.0f};
	ImVec2 pivotAlignment = {0.5f, 0.5f};
	ImVec2 pivotSize = {-1.0f, -1.0f};
	ImVec2 pivotScale = {1.0f, 1.0f};
	float flowMarkerDistance = 50.0f;
	float flowSpeed = 200.0f;
	float flowDuration = 1.0f;

public:
	Stylesheet(std::string name = "default") : name(name)
	{
	}
	Stylesheet(Stylesheet&&) = default;
	Stylesheet(Stylesheet const&) = default;
	Stylesheet& operator=(Stylesheet&&) = default;
	Stylesheet& operator=(Stylesheet const&) = default;
	~Stylesheet() = default;

public:
	static void addSheet(Stylesheet&& sheet)
	{
		assert(sheets.find(sheet.getName()) == sheets.end());
		sheets[sheet.getName()] = std::move(sheet);
		if(currentSheet == nullptr)
			setCurrentSheet(sheets.begin()->first);
	}

	static Stylesheet& getSheet(std::string name)
	{
		assert(sheets.find(name) != sheets.end());
		return sheets[name];
	}

	static void setCurrentSheet(std::string name)
	{
		currentSheetName = name;
		currentSheet = &getSheet(name);
	}

	static Stylesheet& getCurrentSheet()
	{
		assert(currentSheet != nullptr);
		return *currentSheet;
	}

	std::string getName() const
	{
		return name;
	}
};

} // namespace rshp::gui