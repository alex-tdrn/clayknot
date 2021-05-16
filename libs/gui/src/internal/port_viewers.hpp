#pragma once
#include "rsp/base/port.hpp"
#include "rsp/gui/widgets/viewer.hpp"
#include "rsp/util/color_rgba.hpp"

#include <imnodes.h>
#include <memory>

namespace rsp::gui::impl
{
class port_viewer
{
public:
	port_viewer() = delete;
	port_viewer(rsp::port const* port, int id);
	port_viewer(port_viewer const&) = delete;
	port_viewer(port_viewer&&) noexcept = delete;
	auto operator=(port_viewer const&) -> port_viewer& = delete;
	auto operator=(port_viewer&&) noexcept -> port_viewer& = delete;
	virtual ~port_viewer() = default;

	auto get_id() const -> int;
	virtual auto get_port() const -> rsp::port const* = 0;
	virtual void draw() = 0;

protected:
	int id = -1; // NOLINT
	std::uint32_t color; // NOLINT
	std::unique_ptr<rsp::gui::viewer> data_viewer; // NOLINT
};

class input_port_viewer final : public port_viewer
{
public:
	input_port_viewer() = delete;
	input_port_viewer(rsp::input_port const* port, int id);
	input_port_viewer(input_port_viewer const&) = delete;
	input_port_viewer(input_port_viewer&&) noexcept = delete;
	auto operator=(input_port_viewer const&) -> input_port_viewer& = delete;
	auto operator=(input_port_viewer&&) noexcept -> input_port_viewer& = delete;
	~input_port_viewer() final = default;

	auto get_port() const -> rsp::input_port const* final;
	void draw() final;

private:
	rsp::input_port const* port = nullptr;
};

class output_port_viewer final : public port_viewer
{
public:
	output_port_viewer() = delete;
	output_port_viewer(rsp::output_port const* port, int id);
	output_port_viewer(output_port_viewer const&) = delete;
	output_port_viewer(output_port_viewer&&) noexcept = delete;
	auto operator=(output_port_viewer const&) -> output_port_viewer& = delete;
	auto operator=(output_port_viewer&&) noexcept -> output_port_viewer& = delete;
	~output_port_viewer() final = default;

	auto get_port() const -> rsp::output_port const* final;
	void draw() final;

private:
	rsp::output_port const* port = nullptr;
};

inline port_viewer::port_viewer(rsp::port const* port, int id) : id(id)
{
	data_viewer = rsp::gui::viewer::create(port->get_data_type_hash(), port->get_data_pointer(), port->get_name());
	data_viewer->set_maximum_width(200);
	color = color_rgba(color_rgb::create_random(port->get_data_type_hash()), 1.0f).packed();
}

inline auto port_viewer::get_id() const -> int
{
	return id;
}

inline input_port_viewer::input_port_viewer(rsp::input_port const* port, int id) : port_viewer(port, id), port(port)
{
}

inline auto input_port_viewer::get_port() const -> rsp::input_port const*
{
	return port;
}

inline void input_port_viewer::draw()
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

inline output_port_viewer::output_port_viewer(rsp::output_port const* port, int id) : port_viewer(port, id), port(port)
{
}

inline auto output_port_viewer::get_port() const -> output_port const*
{
	return port;
}

inline void output_port_viewer::draw()
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

inline auto create_port_viewer(rsp::port const* port, int id) -> std::unique_ptr<port_viewer>
{
	if(auto const* inputPort = dynamic_cast<rsp::input_port const*>(port); inputPort != nullptr)
		return std::make_unique<input_port_viewer>(inputPort, id);
	else if(auto const* outputPort = dynamic_cast<rsp::output_port const*>(port); outputPort != nullptr)
		return std::make_unique<output_port_viewer>(outputPort, id);
	return nullptr;
}

} // namespace rsp::gui::impl