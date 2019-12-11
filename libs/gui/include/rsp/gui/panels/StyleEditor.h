#pragma once

#include "rsp/gui/Stylesheet.hpp"
#include "rsp/gui/panels/Panel.h"
#include "rsp/gui/widgets/Widget.hpp"

#include <memory>

namespace rsp::gui
{
class StyleEditor final : public Panel
{
private:
	Stylesheet* sheet;
	std::vector<std::unique_ptr<Widget>> styleWidgets;

public:
	StyleEditor();
	StyleEditor(StyleEditor const&) = delete;
	StyleEditor(StyleEditor&&) = default;
	StyleEditor& operator=(StyleEditor const&) = delete;
	StyleEditor& operator=(StyleEditor&&) = default;
	~StyleEditor() = default;

private:
	void setStylesheet(Stylesheet& sheet);
	void drawContents() override;
};

} // namespace rsp::gui