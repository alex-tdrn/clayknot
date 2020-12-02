#pragma once

#include "rsp/gui/Stylesheet.hpp"
#include "rsp/gui/panels/Panel.h"
#include "rsp/gui/widgets/Widget.hpp"

#include <memory>
#include <vector>

namespace rsp::gui
{
class StyleEditor final : public Panel
{
public:
	StyleEditor();
	StyleEditor(StyleEditor const&) = delete;
	StyleEditor(StyleEditor&&) = default;
	auto operator=(StyleEditor const&) -> StyleEditor& = delete;
	auto operator=(StyleEditor&&) -> StyleEditor& = delete;
	~StyleEditor() final = default;

private:
	Stylesheet* sheet = nullptr;
	std::vector<std::unique_ptr<Widget>> styleWidgets;

	void setStylesheet(Stylesheet& sheet);
	void drawContents() override;
};

} // namespace rsp::gui