#include "rshp/gui/panels/Panel.h"

namespace rshp::gui
{
	
	void Panel::draw()
	{
		if(!visible)
			return;
		drawContents();
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

}