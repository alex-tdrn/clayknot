#pragma once

#include <imgui.h>
#include <random>
#include <unordered_map>

namespace ImGui
{
inline void DrawCircle(ImVec2 pos, float radius, ImVec4 fillColor, ImVec4 contourColor = {0, 0, 0, 1})
{
	auto* drawList = ImGui::GetWindowDrawList();
	const float contourThickness = 0.25f * radius;
	drawList->AddCircleFilled(pos, radius, ImGui::GetColorU32(fillColor));
	drawList->AddCircle(pos, radius, ImGui::GetColorU32(contourColor), 12, contourThickness);
}

inline void DrawDiamond(ImVec2 center, float size, ImVec4 fillColor, ImVec4 contourColor = {0, 0, 0, 1})
{
	auto* drawList = ImGui::GetWindowDrawList();
	const float contourThickness = 0.25f * size;

	drawList->PathLineTo(ImVec2{center.x, center.y - size});
	drawList->PathLineTo(ImVec2{center.x + size, center.y});
	drawList->PathLineTo(ImVec2{center.x, center.y + size});
	drawList->PathLineTo(ImVec2{center.x - size, center.y});
	drawList->AddConvexPolyFilled(drawList->_Path.Data, drawList->_Path.Size, ImGui::GetColorU32(fillColor));
	drawList->PathStroke(ImGui::GetColorU32(contourColor), true, contourThickness);
}

inline auto ColorFromHash(std::size_t hash) -> ImVec4
{
	static std::unordered_map<std::size_t, ImVec4> colorMap;
	if(colorMap.find(hash) == colorMap.end())
	{
		std::mt19937 generator(hash);
		std::uniform_real_distribution<float> dis(0, 1);

		colorMap[hash] = {dis(generator), dis(generator), dis(generator), 1.0f};
	}
	return colorMap[hash];
}

} // namespace ImGui