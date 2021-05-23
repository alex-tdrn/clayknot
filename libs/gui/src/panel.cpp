#include "clk/gui/panel.h"
#include "clk/gui/widgets/widget.hpp"

#include <algorithm>
#include <iterator>

namespace clk::gui
{
panel::panel(std::string title) : _title(std::move(title))
{
}

panel::panel(panel const& other)
{
	*this = other;
}

auto panel::operator=(panel const& other) -> panel&
{
	if(this != &other)
	{
		_title = "Copy of" + other._title;
		_visible = other._visible;
		_flags = other._flags;
		_widgets.clear();
		_widgets.reserve(other._widgets.size());
		std::transform(
			other._widgets.cbegin(), other._widgets.cend(), std::back_inserter(_widgets), [](auto const& widget) {
				return widget->clone();
			});
	}
	return *this;
}

void panel::add_widget(std::unique_ptr<clk::gui::widget>&& widget)
{
	_widgets.push_back(std::move(widget));
}

void panel::draw()
{
	if(!_visible)
		return;
	ImGui::PushID(this);
	if(ImGui::Begin(_title.c_str(), &_visible, _flags))
	{
		for(auto& widget : _widgets)
			widget->draw();
	}
	ImGui::End();
	ImGui::PopID();
}

void panel::set_title(std::string title)
{
	_title = std::move(title);
}

auto panel::get_title() const -> std::string const&
{
	return _title;
}

auto panel::is_visible() const -> bool
{
	return _visible;
}

void panel::toggle_visibility()
{
	_visible = !_visible;
}

void panel::show()
{
	_visible = true;
}

void panel::hide()
{
	_visible = false;
}

} // namespace clk::gui