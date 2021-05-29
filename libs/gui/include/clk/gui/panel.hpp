#pragma once

#include <imgui.h>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace clk::gui
{
class widget;
class panel final
{
public:
	panel() = default;
	explicit panel(std::string title);
	panel(panel const& other);
	panel(panel&&) noexcept = default;
	auto operator=(panel const& other) -> panel&;
	auto operator=(panel&&) noexcept -> panel& = default;
	~panel() = default;

	void add_widget(std::unique_ptr<clk::gui::widget>&& widget);
	void draw();
	void set_title(std::string title);
	auto title() const -> std::string_view;
	auto visible() const -> bool;
	void toggle_visibility();
	void show();
	void hide();

private:
	std::string _title;
	bool _visible = true;
	ImGuiWindowFlags _flags = ImGuiWindowFlags_None;
	std::vector<std::unique_ptr<clk::gui::widget>> _widgets;
};

} // namespace clk::gui