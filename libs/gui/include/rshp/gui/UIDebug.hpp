#pragma once

#include <imgui.h>

namespace rshp::gui::debug
{
static inline void drawItemRect(ImDrawList* drawList, ImVec4 color = {1, 0, 1, 1})
{
#ifdef DEBUG
	auto min = ImGui::GetItemRectMin();
	auto max = ImGui::GetItemRectMax();

	const float minSize = 4;
	if(max.x - min.x < minSize)
		max.x = min.x + minSize;
	if(max.y - min.y < minSize)
		max.y = min.y + minSize;

	drawList->AddRect(min, max, ImGui::GetColorU32(color));
#endif
}

static inline void drawSpacingRect(ImDrawList* drawList, ImVec4 color = {1, 1, 0, 1})
{
#ifdef DEBUG
	auto cursor = ImGui::GetCursorPos();
	drawList->AddRect(
		{
			cursor.x - ImGui::GetStyle().ItemSpacing.x,
			cursor.y,
		},
		{cursor.x, cursor.y + ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y}, ImGui::GetColorU32(color));
#endif
}

static inline void drawRect(ImDrawList* drawList, ImVec2 start, float w, ImVec4 color = {0, 1, 1, 1})
{
#ifdef DEBUG
	drawList->AddRect(start, {start.x + w, start.y + ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y},
		ImGui::GetColorU32(color));
#endif
}

static inline void drawRect(ImDrawList* drawList, ImVec2 start, float w, float h, ImVec4 color = {0, 1, 1, 1})
{
#ifdef DEBUG
	drawList->AddRect(start, {start.x + w, start.y + h}, ImGui::GetColorU32(color));
#endif
}

} // namespace rshp::gui::debug