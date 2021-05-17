#include "clk/gui/panel.h"
#include "clk/gui/widgets/widget.hpp"

#include <algorithm>
#include <iterator>

namespace clk::gui
{
panel::panel(std::string title) : title(std::move(title))
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
		this->title = "Copy of" + other.title;
		this->visible = other.visible;
		this->flags = other.flags;
		widgets.clear();
		widgets.reserve(other.widgets.size());
		std::transform(
			other.widgets.cbegin(), other.widgets.cend(), std::back_inserter(widgets), [](auto const& widget) {
				return widget->clone();
			});
	}
	return *this;
}

void panel::add_widget(std::unique_ptr<clk::gui::widget>&& widget)
{
	widgets.push_back(std::move(widget));
}

void panel::draw()
{
	if(!visible)
		return;
	ImGui::PushID(this);
	if(ImGui::Begin(title.c_str(), &visible, flags))
	{
		for(auto& widget : widgets)
			widget->draw();
	}
	ImGui::End();
	ImGui::PopID();
}

void panel::set_title(std::string title)
{
	this->title = std::move(title);
}

auto panel::get_title() const -> std::string const&
{
	return title;
}

auto panel::is_visible() const -> bool
{
	return visible;
}

void panel::toggle_visibility()
{
	visible = !visible;
}

void panel::show()
{
	visible = true;
}

void panel::hide()
{
	visible = false;
}

} // namespace clk::gui