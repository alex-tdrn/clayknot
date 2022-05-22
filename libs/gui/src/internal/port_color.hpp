#pragma once

#include <clk/base/port.hpp>
#include <clk/util/color_rgb.hpp>
#include <clk/util/color_rgba.hpp>

namespace clk::gui::impl
{

inline auto port_color(clk::port const* port) -> std::uint32_t
{
	return color_rgba(color_rgb::create_random(port->data_type_hash()), 1.0f).packed();
}

} // namespace clk::gui::impl