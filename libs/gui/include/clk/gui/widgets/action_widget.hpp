#pragma once

#include "clk/gui/widgets/widget.hpp"

#include <functional>
#include <memory>
#include <string_view>

namespace clk::gui
{

class action_widget : public widget
{
public:
	action_widget() = delete;
	action_widget(std::function<void()> action, std::string_view name);
	action_widget(action_widget const&) = delete;
	action_widget(action_widget&&) = delete;
	auto operator=(action_widget const&) -> action_widget& = delete;
	auto operator=(action_widget&&) -> action_widget& = delete;
	~action_widget() override = default;

	auto clone() const -> std::unique_ptr<widget> override;
	void copy(widget const& other) override;

	void draw_contents() const override;

private:
	std::function<void()> _action;
};

} // namespace clk::gui