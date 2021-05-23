#pragma once
#include "clk/base/port.hpp"
#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/viewer.hpp"
#include "clk/util/color_rgba.hpp"

#include <imnodes.h>
#include <memory>

namespace clk::gui::impl
{
class port_editor
{
public:
	port_editor() = delete;
	port_editor(clk::port* port, int id);
	port_editor(port_editor const&) = delete;
	port_editor(port_editor&&) noexcept = delete;
	auto operator=(port_editor const&) -> port_editor& = delete;
	auto operator=(port_editor&&) noexcept -> port_editor& = delete;
	virtual ~port_editor() = default;

	auto get_id() const -> int;
	auto get_color() const -> std::uint32_t;
	void set_enabled(bool enabled);
	void set_stable_height(bool stable_height);
	virtual auto get_port() const -> port* = 0;
	virtual void draw(clk::gui::widget* override_widget = nullptr) = 0;

protected:
	int _id = -1; // NOLINT
	std::uint32_t _color; // NOLINT
	std::unique_ptr<clk::gui::viewer> _data_viewer; // NOLINT
	bool _enabled = true; // NOLINT
	bool _stable_height = false; // NOLINT
};

class input_port_editor final : public port_editor
{
public:
	input_port_editor() = delete;
	input_port_editor(clk::input_port* port, int id);
	input_port_editor(input_port_editor const&) = delete;
	input_port_editor(input_port_editor&&) noexcept = delete;
	auto operator=(input_port_editor const&) -> input_port_editor& = delete;
	auto operator=(input_port_editor&&) noexcept -> input_port_editor& = delete;
	~input_port_editor() final = default;

	auto get_port() const -> input_port* final;
	void draw(clk::gui::widget* override_widget = nullptr) final;

private:
	clk::input_port* _port = nullptr;
	std::unique_ptr<clk::gui::editor> _default_data_editor;
};

class output_port_editor final : public port_editor
{
public:
	output_port_editor() = delete;
	output_port_editor(clk::output_port* port, int id);
	output_port_editor(output_port_editor const&) = delete;
	output_port_editor(output_port_editor&&) noexcept = delete;
	auto operator=(output_port_editor const&) -> output_port_editor& = delete;
	auto operator=(output_port_editor&&) noexcept -> output_port_editor& = delete;
	~output_port_editor() final = default;

	auto get_port() const -> output_port* final;
	void draw(clk::gui::widget* override_widget = nullptr) final;

private:
	clk::output_port* _port = nullptr;
};

inline port_editor::port_editor(clk::port* port, int id) : _id(id)
{
	_data_viewer = clk::gui::viewer::create(port->get_data_type_hash(), port->get_data_pointer(), port->get_name());
	_data_viewer->set_maximum_width(200);
	_color = color_rgba(color_rgb::create_random(port->get_data_type_hash()), 1.0f).packed();
}

inline auto port_editor::get_id() const -> int
{
	return _id;
}

inline auto port_editor::get_color() const -> std::uint32_t
{
	return _color;
}

inline void port_editor::set_enabled(bool enabled)
{
	_enabled = enabled;
}

inline void port_editor::set_stable_height(bool stableHeight)
{
	_stable_height = stableHeight;
}

inline input_port_editor::input_port_editor(clk::input_port* port, int id) : port_editor(port, id), _port(port)
{
	auto* default_port = &port->get_default_port();

	_default_data_editor = clk::gui::editor::create(
		default_port->get_data_type_hash(), default_port->get_data_pointer(), port->get_name(), [=]() {
			default_port->update_timestamp();
			default_port->push();
		});
	_default_data_editor->set_maximum_width(200);
}

inline auto input_port_editor::get_port() const -> input_port*
{
	return _port;
}

inline void input_port_editor::draw(clk::gui::widget* override_widget)
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, _color);

	float const begin_y = ImGui::GetCursorPosY();

	if(!_enabled)
		imnodes::BeginStaticAttribute(_id);
	else if(_port->is_connected())
		imnodes::BeginInputAttribute(_id, imnodes::PinShape_QuadFilled);
	else
		imnodes::BeginInputAttribute(_id, imnodes::PinShape_Quad);

	if(override_widget != nullptr)
	{
		override_widget->draw();
	}
	else
	{
		if(!_port->is_connected())
		{
			_default_data_editor->draw();
		}
		else
		{
			_data_viewer->update_data_pointer(_port->get_data_pointer());
			_data_viewer->draw();
		}

		if(_stable_height)
		{
			float current_height = ImGui::GetCursorPosY() - begin_y;
			float max_height = std::max(_data_viewer->get_last_size().y, _default_data_editor->get_last_size().y);
			if(current_height < max_height)
				ImGui::Dummy(ImVec2(10, max_height - current_height));
		}
	}

	if(!_enabled)
		imnodes::EndStaticAttribute();
	else
		imnodes::EndInputAttribute();

	imnodes::PopColorStyle();
}

inline output_port_editor::output_port_editor(clk::output_port* port, int id) : port_editor(port, id), _port(port)
{
}

inline auto output_port_editor::get_port() const -> output_port*
{
	return _port;
}

inline void output_port_editor::draw(clk::gui::widget* override_widget)
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, _color);

	if(!_enabled)
		imnodes::BeginStaticAttribute(_id);
	else if(_port->is_connected())
		imnodes::BeginOutputAttribute(_id, imnodes::PinShape_TriangleFilled);
	else
		imnodes::BeginOutputAttribute(_id, imnodes::PinShape_Triangle);

	if(override_widget != nullptr)
		override_widget->draw();
	else
		_data_viewer->draw();

	if(!_enabled)
		imnodes::EndStaticAttribute();
	else
		imnodes::EndOutputAttribute();

	imnodes::PopColorStyle();
}

inline auto create_port_editor(clk::port* port, int id) -> std::unique_ptr<port_editor>
{
	if(auto* input_port = dynamic_cast<clk::input_port*>(port); input_port != nullptr)
		return std::make_unique<input_port_editor>(input_port, id);
	else if(auto* output_port = dynamic_cast<clk::output_port*>(port); output_port != nullptr)
		return std::make_unique<output_port_editor>(output_port, id);
	return nullptr;
}

} // namespace clk::gui::impl