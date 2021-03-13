#pragma once

#include <imgui.h>
#include <memory>
#include <string>
#include <vector>

namespace rsp::gui
{
class Widget;
class Panel final
{
public:
	Panel() = default;
	explicit Panel(std::string title);
	Panel(Panel const&);
	Panel(Panel&&) noexcept = default;
	auto operator=(Panel const&) -> Panel&;
	auto operator=(Panel&&) noexcept -> Panel& = default;
	~Panel() = default;

	void addWidget(std::unique_ptr<Widget>&& widget);
	void draw();
	void setTitle(std::string title);
	auto getTitle() const -> std::string const&;
	auto isVisible() const -> bool;
	void toggleVisibility();
	void show();
	void hide();

private:
	std::string title;
	bool visible = true;
	ImGuiWindowFlags flags = ImGuiWindowFlags_None;
	std::vector<std::unique_ptr<Widget>> widgets;
};

} // namespace rsp::gui