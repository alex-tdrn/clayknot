#pragma once

#include "rsp/gui/panels/Panel.h"

#include <memory>
#include <string>
#include <vector>

namespace rsp::gui
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
	auto operator=(RootPanel const&) -> RootPanel& = delete;
	auto operator=(RootPanel&&) -> RootPanel& = delete;
	~RootPanel() final = default;

private:
	auto addChildImpl(std::unique_ptr<Panel>&& widget) -> Panel*;
	void drawContents() override;

public:
	template <typename W>
	auto addChild(std::unique_ptr<W>&& widget, bool visible = true) -> W*
	{
		static_assert(std::is_base_of_v<Panel, W>);
		if(visible)
			widget->show();
		else
			widget->hide();
		auto* ret = widget.get();
		addChildImpl(std::move(widget));
		return ret;
	}
};

} // namespace rsp::gui