#pragma once
#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/viewer.hpp"

#include <cstdint>
#include <glm/glm.hpp>
#include <memory>

namespace clk
{
class port;
}

namespace clk::gui
{
class widget;
class widget_factory;
} // namespace clk::gui

namespace clk::gui::impl
{

class port_editor
{
public:
	port_editor() = delete;
	port_editor(clk::port* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets);
	port_editor(port_editor const&) = delete;
	port_editor(port_editor&&) noexcept = delete;
	auto operator=(port_editor const&) -> port_editor& = delete;
	auto operator=(port_editor&&) noexcept -> port_editor& = delete;
	virtual ~port_editor() = default;

	auto id() const -> int;
	void set_enabled(bool enabled);
	void set_stable_height(bool stable_height);
	virtual auto port() const -> port* = 0;
	virtual void draw(clk::gui::widget* override_widget = nullptr) = 0;
	auto position() const -> glm::vec2 const&;

protected:
	int _id = -1; // NOLINT
	std::unique_ptr<clk::gui::viewer> _data_viewer; // NOLINT
	bool _enabled = true; // NOLINT
	bool _stable_height = false; // NOLINT
	glm::vec2 _position = {0.0f, 0.0f}; // NOLINT
	bool const& _draw_port_widgets; // NOLINT

	void update_viewer_type();
};

class input_editor final : public port_editor
{
public:
	input_editor() = delete;
	input_editor(clk::input* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets);
	input_editor(input_editor const&) = delete;
	input_editor(input_editor&&) noexcept = delete;
	auto operator=(input_editor const&) -> input_editor& = delete;
	auto operator=(input_editor&&) noexcept -> input_editor& = delete;
	~input_editor() final = default;

	auto port() const -> input* final;
	void draw(clk::gui::widget* override_widget = nullptr) final;

private:
	clk::input* _port = nullptr;
	std::unique_ptr<clk::gui::editor> _default_data_editor;
};

class output_editor final : public port_editor
{
public:
	output_editor() = delete;
	output_editor(clk::output* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets);
	output_editor(output_editor const&) = delete;
	output_editor(output_editor&&) noexcept = delete;
	auto operator=(output_editor const&) -> output_editor& = delete;
	auto operator=(output_editor&&) noexcept -> output_editor& = delete;
	~output_editor() final = default;

	auto port() const -> output* final;
	void draw(clk::gui::widget* override_widget = nullptr) final;

private:
	clk::output* _port = nullptr;
};

auto create_port_editor(clk::port* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
	-> std::unique_ptr<port_editor>;

} // namespace clk::gui::impl