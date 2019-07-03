#pragma once
#include "Window.h"

#include <vector>
#include <memory>

class RootWindow final : public Window
{
private:
	std::vector<std::unique_ptr<Window>> children;
	std::string dockspaceName;

public:
	RootWindow();
	RootWindow(RootWindow const&) = delete;
	RootWindow(RootWindow&&) = default;
	RootWindow& operator=(RootWindow const&) = delete;
	RootWindow& operator=(RootWindow&&) = default;
	virtual ~RootWindow() = default;

private:
	Window* addChildImpl(std::unique_ptr<Window>&& widget);
	void drawContents() override;

public:
	template<typename W>
	W* addChild(std::unique_ptr<W>&& widget, bool visible = true)
	{
		static_assert(std::is_base_of_v<Window, W>);
		if(visible)
			widget->show();
		else
			widget->hide();
		auto ret = widget.get();
		addChildImpl(std::move(widget));
		return ret;
	}
};