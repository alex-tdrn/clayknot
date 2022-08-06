#pragma once

#include "clk/util/color_rgba.hpp"
#include <glm/glm.hpp>
#include <imgui.h>

namespace clk::gui
{

inline auto to_imgui(glm::vec2 glm_vector)
{
	return ImVec2(glm_vector.x, glm_vector.y);
}

inline auto to_imgui(glm::vec4 glm_vector)
{
	return ImVec4(glm_vector.x, glm_vector.y, glm_vector.z, glm_vector.w);
}

inline auto to_imgui(clk::color_rgba color)
{
	return ImVec4(color.r(), color.g(), color.b(), color.a());
}

inline auto to_glm(ImVec2 imgui_vector)
{
	return glm::vec2(imgui_vector.x, imgui_vector.y);
}

inline auto to_glm(ImVec4 imgui_vector)
{
	return glm::vec4(imgui_vector.x, imgui_vector.y, imgui_vector.z, imgui_vector.w);
}

inline auto to_color(ImVec4 imgui_vector)
{
	return clk::color_rgba(imgui_vector.x, imgui_vector.y, imgui_vector.z, imgui_vector.w);
}
} // namespace clk::gui
