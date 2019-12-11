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

std::string const& Panel::getTitle() const
{
	return title;
}

bool Panel::isVisible() const
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