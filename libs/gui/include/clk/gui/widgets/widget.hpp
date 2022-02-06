#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>
#include <tuple>

namespace clk::gui
{
class widget_factory;

class widget_tree;

class widget
{
public:
	widget() = delete;
	widget(std::shared_ptr<widget_factory const> factory, std::string_view name);
	widget(widget const&) = delete;
	widget(widget&&) = delete;
	auto operator=(widget const&) -> widget& = delete;
	auto operator=(widget&&) -> widget& = delete;
	virtual ~widget();

	virtual auto clone() const -> std::unique_ptr<widget> = 0;
	virtual void copy(widget const& other) = 0;

	void draw() const;
	void enable_title();
	void disable_title();
	void prefer_extended();
	void prefer_compact();
	void set_maximum_width(float width);
	void clear_maximum_width();
	auto last_size() const -> glm::vec2;
	void set_name(std::string_view name);
	auto name() const -> std::string_view;
	void set_interactivity(bool interactive) noexcept;
	auto is_interactive() const noexcept -> bool;
	auto get_widget_factory() const -> std::shared_ptr<widget_factory const> const&;
	void set_widget_factory(std::shared_ptr<widget_factory const> factory);
	auto get_settings() const -> widget_tree const*;

protected:
	auto available_width() const -> float;
	auto extended_preferred() const -> bool;
	virtual void draw_contents() const = 0;
	auto settings() -> widget_tree&;

private:
	std::string _name;
	bool _draw_title = true;
	bool _interactive = true;
	std::shared_ptr<widget_factory const> _factory;
	std::unique_ptr<widget_tree> _settings;
	mutable std::optional<float> _maximum_width;
	mutable glm::vec2 _last_size = {0.0f, 0.0f};
	mutable bool _extended_available = false;
	mutable bool _extended_preferred = false;
};

} // namespace clk::gui
