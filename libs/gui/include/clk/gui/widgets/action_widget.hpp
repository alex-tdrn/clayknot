#pragma once

#include "clk/gui/widgets/widget.hpp"

#include <functional>
#include <imgui.h>
#include <memory>

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

inline action_widget::action_widget(std::function<void()> action, std::string_view name)
	: widget(name), _action(std::move(action))
{
	disable_title();
}

inline auto action_widget::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<action_widget>(this->_action, this->name());
	clone->copy(*this);
	return clone;
}

inline void action_widget::copy(widget const& other)
{
	widget::copy(other);
}

inline void action_widget::draw_contents() const
{
	if(!_action)
	{
		ImGui::Text("NO ACTION SET");
	}
	else if(ImGui::Button(name().data()))
	{
		_action();
	}
}

} // namespace clk::gui