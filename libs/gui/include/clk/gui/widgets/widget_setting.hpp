#pragma once
#include "clk/gui/widgets/widget.hpp"
#include "clk/gui/widgets/widget_factory.hpp"

#include <string>

namespace clk::gui
{
template <typename T>
void widget::register_setting(T& value, std::string name)
{
	_setting_widgets.push_back(_factory->create(data_writer<T>{[&]() {
		return &value;
	}},
		name));
}

} // namespace clk::gui