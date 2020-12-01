#pragma once

#include <imgui.h>
#include <string>

namespace rsp::gui
{
class Panel
{
private:
	static inline int idCounter = 0;

protected:
	const int id = idCounter++;
	std::string title;
	bool visible = true;
	ImGuiWindowFlags flags = ImGuiWindowFlags_None;
	bool customWindowSetup = false;

public:
	Panel() = default;
	Panel(Panel const&) = delete;
	Panel(Panel&&) = default;
	auto operator=(Panel const&) -> Panel& = delete;
	auto operator=(Panel&&) -> Panel& = delete;
	virtual ~Panel() = default;

protected:
	virtual void drawContents() = 0;

public:
	void draw();
	auto getTitle() const -> std::string const&;
	auto isVisible() const -> bool;
	void toggleVisibility();
	void show();
	void hide();
};

} // namespace rsp::gui