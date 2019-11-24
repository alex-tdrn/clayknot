#pragma once

#include "rshp/gui/panels/Panel.h"
#include "rshp/gui/Stylesheet.hpp"

namespace rshp::gui
{
	
	class StyleEditor final : public Panel
	{
	private:
		Stylesheet* sheet;

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

}