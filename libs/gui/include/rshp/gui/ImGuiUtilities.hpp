#pragma once
#include <imgui.h>
#include <unordered_map>

namespace ImGui
{
	static void DrawCircle(ImVec2 pos, float radius, ImVec4 fillColor, ImVec4 contourColor = {0, 0, 0, 1})
	{
		auto drawList = ImGui::GetWindowDrawList();
		const float contourThickness = 0.25 * radius;
		drawList->AddCircleFilled(pos, radius, ImGui::GetColorU32(fillColor));
		drawList->AddCircle(pos, radius, ImGui::GetColorU32(contourColor), 12, contourThickness);
	}

	static void DrawDiamond(ImVec2 center, float size, ImVec4 fillColor, ImVec4 contourColor = {0, 0, 0, 1})
	{
		auto drawList = ImGui::GetWindowDrawList();
		const float contourThickness = 0.25 * size;

		drawList->PathLineTo(ImVec2{center.x, center.y - size});
		drawList->PathLineTo(ImVec2{center.x + size, center.y});
		drawList->PathLineTo(ImVec2{center.x, center.y + size});
		drawList->PathLineTo(ImVec2{center.x - size, center.y});
		drawList->AddConvexPolyFilled(drawList->_Path.Data, drawList->_Path.Size, ImGui::GetColorU32(fillColor));
		drawList->PathStroke(ImGui::GetColorU32(contourColor), true, contourThickness);
	}

	static ImVec4 ColorFromHash(std::size_t hash)
	{
		static std::unordered_map<std::size_t, ImVec4> colorMap;
		if(colorMap.find(hash) == colorMap.end())
		{
			srand(hash);
			const float r = rand() % 256 / 256.0f;
			const float g = rand() % 256 / 256.0f;
			const float b = rand() % 256 / 256.0f;
			colorMap[hash] = {r, g, b, 1.0f};
		}
		return colorMap[hash];
	}

}