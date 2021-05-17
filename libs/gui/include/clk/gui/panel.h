#pragma once

#include <imgui.h>
#include <memory>
#include <string>
#include <vector>

namespace clk::gui
{
class widget;
class panel final
{
public:
	panel() = default;
	explicit panel(std::string title);
	panel(panel const&);
	panel(panel&&) noexcept = default;
	auto operator=(panel const&) -> panel&;
	auto operator=(panel&&) noexcept -> panel& = default;
	~panel() = default;

	void add_widget(std::unique_ptr<clk::gui::widget>&& widget);
	void draw();
	void set_title(std::string title);
	auto get_title() const -> std::string const&;
	auto is_visible() const -> bool;
	void toggle_visibility();
	void show();
	void hide();

private:
	std::string title;
	bool visible = true;
	ImGuiWindowFlags flags = ImGuiWindowFlags_None;
	std::vector<std::unique_ptr<clk::gui::widget>> widgets;
};

} // namespace clk::gui