#pragma once

#include "rshp/gui/panels/Panel.h"

#include <memory>
#include <vector>

namespace rshp::gui
{
class RootPanel final : public Panel
{
private:
	std::vector<std::unique_ptr<Panel>> children;
	std::string dockspaceName;

public:
	RootPanel();
	RootPanel(RootPanel const&) = delete;
	RootPanel(RootPanel&&) = default;
	RootPanel& operator=(RootPanel const&) = delete;
	RootPanel& operator=(RootPanel&&) = default;
	virtual ~RootPanel() = default;

private:
	Panel* addChildImpl(std::unique_ptr<Panel>&& widget);
	void drawContents() override;

public:
	template <typename W>
	W* addChild(std::unique_ptr<W>&& widget, bool visible = true)
	{
		static_assert(std::is_base_of_v<Panel, W>);
		if(visible)
			widget->show();
		else
			widget->hide();
		auto ret = widget.get();
		addChildImpl(std::move(widget));
		return ret;
	}
};

} // namespace rshp::gui