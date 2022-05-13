#pragma once
#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
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
class widget_factory;
}

namespace clk::gui::impl
{
class port_viewer
{
public:
	port_viewer() = delete;
	port_viewer(clk::port const* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets);
	port_viewer(port_viewer const&) = delete;
	port_viewer(port_viewer&&) noexcept = delete;
	auto operator=(port_viewer const&) -> port_viewer& = delete;
	auto operator=(port_viewer&&) noexcept -> port_viewer& = delete;
	virtual ~port_viewer() = default;

	auto id() const -> int;
	virtual auto port() const -> clk::port const* = 0;
	virtual void draw() = 0;
	auto position() const -> glm::vec2 const&;

protected:
	int _id = -1; // NOLINT
	std::uint32_t _color; // NOLINT
	std::unique_ptr<clk::gui::viewer> _data_viewer; // NOLINT
	glm::vec2 _position = {0.0f, 0.0f}; // NOLINT
	bool const& _draw_port_widgets; // NOLINT
};

class input_viewer final : public port_viewer
{
public:
	input_viewer() = delete;
	input_viewer(clk::input const* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets);
	input_viewer(input_viewer const&) = delete;
	input_viewer(input_viewer&&) noexcept = delete;
	auto operator=(input_viewer const&) -> input_viewer& = delete;
	auto operator=(input_viewer&&) noexcept -> input_viewer& = delete;
	~input_viewer() final = default;

	auto port() const -> clk::input const* final;
	void draw() final;

private:
	clk::input const* _port = nullptr;
};

class output_viewer final : public port_viewer
{
public:
	output_viewer() = delete;
	output_viewer(clk::output const* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets);
	output_viewer(output_viewer const&) = delete;
	output_viewer(output_viewer&&) noexcept = delete;
	auto operator=(output_viewer const&) -> output_viewer& = delete;
	auto operator=(output_viewer&&) noexcept -> output_viewer& = delete;
	~output_viewer() final = default;

	auto port() const -> clk::output const* final;
	void draw() final;

private:
	clk::output const* _port = nullptr;
};

auto create_port_viewer(clk::port const* port, int id, widget_factory const& widget_factory,
	bool const& draw_port_widgets) -> std::unique_ptr<port_viewer>;

} // namespace clk::gui::impl