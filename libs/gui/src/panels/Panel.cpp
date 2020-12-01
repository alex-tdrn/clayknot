#include "rsp/gui/panels/Panel.h"

namespace rsp::gui
{
void Panel::draw()
{
	if(!visible)
		return;
	if(customWindowSetup)
	{
		drawContents();
	}
	else
	{
		if(ImGui::Begin(title.c_str(), &visible, flags))
			drawContents();
		ImGui::End();
	}
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