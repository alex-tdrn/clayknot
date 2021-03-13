#include "rsp/gui/Panel.h"
#include "rsp/gui/widgets/Widget.hpp"

#include <algorithm>
#include <iterator>

namespace rsp::gui
{
Panel::Panel(std::string title) : title(std::move(title))
{
}

Panel::Panel(Panel const& other)
{
	*this = other;
}

auto Panel::operator=(Panel const& other) -> Panel&
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

void Panel::addWidget(std::unique_ptr<Widget>&& widget)
{
	widgets.push_back(std::move(widget));
}

void Panel::draw()
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

void Panel::setTitle(std::string title)
{
	this->title = std::move(title);
}

auto Panel::getTitle() const -> std::string const&
{
	return title;
}

auto Panel::isVisible() const -> bool
{
	return visible;
}

void Panel::toggleVisibility()
{
	visible = !visible;
}

void Panel::show()
{
	visible = true;
}

void Panel::hide()
{
	visible = false;
}

} // namespace rsp::gui