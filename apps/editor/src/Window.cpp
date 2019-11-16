#include "Window.h"

void Window::draw()
{
	if(!visible)
		return;
	drawContents();
}

std::string const& Window::getTitle() const
{
	return title;
}

bool Window::isVisible() const
{
	return visible;
}

void Window::toggleVisibility()
{
	visible = !visible;
}

void Window::show()
{
	visible = true;
}

void Window::hide()
{
	visible = false;
}
