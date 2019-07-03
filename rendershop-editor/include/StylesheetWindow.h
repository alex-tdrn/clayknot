#pragma once

#include "Window.h"
#include "Stylesheet.hpp"


class StylesheetWindow final : public Window
{
private:
	Stylesheet* sheet;

public:
	StylesheetWindow();
	StylesheetWindow(StylesheetWindow const&) = delete;
	StylesheetWindow(StylesheetWindow&&) = default;
	StylesheetWindow& operator=(StylesheetWindow const&) = delete;
	StylesheetWindow& operator=(StylesheetWindow&&) = default;
	~StylesheetWindow() = default;

private:
	void setStylesheet(Stylesheet& sheet);
	void drawContents() override;

};