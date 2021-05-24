#pragma once
#include "clk/base/port.hpp"
#include "clk/gui/widgets/viewer.hpp"
#include "clk/util/color_rgba.hpp"

#include <imnodes.h>
#include <memory>

namespace clk::gui::impl
{
class port_viewer
{
public:
	port_viewer() = delete;
	port_viewer(clk::port const* port, int id);
	port_viewer(port_viewer const&) = delete;
	port_viewer(port_viewer&&) noexcept = delete;
	auto operator=(port_viewer const&) -> port_viewer& = delete;
	auto operator=(port_viewer&&) noexcept -> port_viewer& = delete;
	virtual ~port_viewer() = default;

	auto get_id() const -> int;
	virtual auto get_port() const -> clk::port const* = 0;
	virtual void draw() = 0;

protected:
	int id = -1; // NOLINT
	std::uint32_t color; // NOLINT
	std::unique_ptr<clk::gui::viewer> data_viewer; // NOLINT
};

class input_viewer final : public port_viewer
{
public:
	input_viewer() = delete;
	input_viewer(clk::input const* port, int id);
	input_viewer(input_viewer const&) = delete;
	input_viewer(input_viewer&&) noexcept = delete;
	auto operator=(input_viewer const&) -> input_viewer& = delete;
	auto operator=(input_viewer&&) noexcept -> input_viewer& = delete;
	~input_viewer() final = default;

	auto get_port() const -> clk::input const* final;
	void draw() final;

private:
	clk::input const* port = nullptr;
};

class output_viewer final : public port_viewer
{
public:
	output_viewer() = delete;
	output_viewer(clk::output const* port, int id);
	output_viewer(output_viewer const&) = delete;
	output_viewer(output_viewer&&) noexcept = delete;
	auto operator=(output_viewer const&) -> output_viewer& = delete;
	auto operator=(output_viewer&&) noexcept -> output_viewer& = delete;
	~output_viewer() final = default;

	auto get_port() const -> clk::output const* final;
	void draw() final;

private:
	clk::output const* port = nullptr;
};

inline port_viewer::port_viewer(clk::port const* port, int id) : id(id)
{
	data_viewer = clk::gui::viewer::create(port->get_data_type_hash(), port->get_data_pointer(), port->get_name());
	data_viewer->set_maximum_width(200);
	color = color_rgba(color_rgb::create_random(port->get_data_type_hash()), 1.0f).packed();
}

inline auto port_viewer::get_id() const -> int
{
	return id;
}

inline input_viewer::input_viewer(clk::input const* port, int id) : port_viewer(port, id), port(port)
{
}

inline auto input_viewer::get_port() const -> clk::input const*
{
	return port;
}

inline void input_viewer::draw()
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, color);
	imnodes::PushColorStyle(imnodes::ColorStyle_PinHovered, color);

	if(port->is_connected())
		imnodes::BeginInputAttribute(id, imnodes::PinShape_QuadFilled);
	else
		imnodes::BeginInputAttribute(id, imnodes::PinShape_Quad);

	data_viewer->update_data_pointer(port->get_data_pointer());
	data_viewer->draw();

	imnodes::EndInputAttribute();

	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
}

inline output_viewer::output_viewer(clk::output const* port, int id) : port_viewer(port, id), port(port)
{
}

inline auto output_viewer::get_port() const -> output const*
{
	return port;
}

inline void output_viewer::draw()
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, color);
	imnodes::PushColorStyle(imnodes::ColorStyle_PinHovered, color);

	if(port->is_connected())
		imnodes::BeginOutputAttribute(id, imnodes::PinShape_TriangleFilled);
	else
		imnodes::BeginOutputAttribute(id, imnodes::PinShape_Triangle);

	data_viewer->draw();

	imnodes::EndOutputAttribute();

	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
}

inline auto create_port_viewer(clk::port const* port, int id) -> std::unique_ptr<port_viewer>
{
	if(auto const* inputPort = dynamic_cast<clk::input const*>(port); inputPort != nullptr)
		return std::make_unique<input_viewer>(inputPort, id);
	else if(auto const* outputPort = dynamic_cast<clk::output const*>(port); outputPort != nullptr)
		return std::make_unique<output_viewer>(outputPort, id);
	return nullptr;
}

} // namespace clk::gui::impl