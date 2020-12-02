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
public:
	RootPanel();
	RootPanel(RootPanel const&) = delete;
	RootPanel(RootPanel&&) = default;
	auto operator=(RootPanel const&) -> RootPanel& = delete;
	auto operator=(RootPanel&&) -> RootPanel& = delete;
	~RootPanel() final = default;

	template <typename W>
	auto addChild(std::unique_ptr<W>&& widget, bool visible = true) -> W*;

private:
	std::vector<std::unique_ptr<Panel>> children;
	std::string dockspaceName;

	auto addChildImpl(std::unique_ptr<Panel>&& widget) -> Panel*;
	void drawContents() override;
};

template <typename W>
auto RootPanel::addChild(std::unique_ptr<W>&& widget, bool visible) -> W*
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

} // namespace rsp::gui