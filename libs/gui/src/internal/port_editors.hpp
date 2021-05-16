#pragma once
#include "rsp/base/port.hpp"
#include "rsp/gui/widgets/editor.hpp"
#include "rsp/gui/widgets/viewer.hpp"
#include "rsp/util/color_rgba.hpp"

#include <imnodes.h>
#include <memory>

namespace rsp::gui::impl
{
class port_editor
{
public:
	port_editor() = delete;
	port_editor(rsp::port* port, int id);
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
	virtual void draw(rsp::gui::widget* override_widget = nullptr) = 0;

protected:
	int id = -1; // NOLINT
	std::uint32_t color; // NOLINT
	std::unique_ptr<rsp::gui::viewer> data_viewer; // NOLINT
	bool enabled = true; // NOLINT
	bool stable_height = false; // NOLINT
};

class input_port_editor final : public port_editor
{
public:
	input_port_editor() = delete;
	input_port_editor(rsp::input_port* port, int id);
	input_port_editor(input_port_editor const&) = delete;
	input_port_editor(input_port_editor&&) noexcept = delete;
	auto operator=(input_port_editor const&) -> input_port_editor& = delete;
	auto operator=(input_port_editor&&) noexcept -> input_port_editor& = delete;
	~input_port_editor() final = default;

	auto get_port() const -> input_port* final;
	void draw(rsp::gui::widget* override_widget = nullptr) final;

private:
	rsp::input_port* port = nullptr;
	std::unique_ptr<rsp::gui::editor> default_data_editor;
};

class output_port_editor final : public port_editor
{
public:
	output_port_editor() = delete;
	output_port_editor(rsp::output_port* port, int id);
	output_port_editor(output_port_editor const&) = delete;
	output_port_editor(output_port_editor&&) noexcept = delete;
	auto operator=(output_port_editor const&) -> output_port_editor& = delete;
	auto operator=(output_port_editor&&) noexcept -> output_port_editor& = delete;
	~output_port_editor() final = default;

	auto get_port() const -> output_port* final;
	void draw(rsp::gui::widget* override_widget = nullptr) final;

private:
	rsp::output_port* port = nullptr;
};

inline port_editor::port_editor(rsp::port* port, int id) : id(id)
{
	data_viewer = rsp::gui::viewer::create(port->get_data_type_hash(), port->get_data_pointer(), port->get_name());
	data_viewer->set_maximum_width(200);
	color = color_rgba(color_rgb::create_random(port->get_data_type_hash()), 1.0f).packed();
}

inline auto port_editor::get_id() const -> int
{
	return id;
}

inline auto port_editor::get_color() const -> std::uint32_t
{
	return color;
}

inline void port_editor::set_enabled(bool enabled)
{
	this->enabled = enabled;
}

inline void port_editor::set_stable_height(bool stableHeight)
{
	this->stable_height = stableHeight;
}

inline input_port_editor::input_port_editor(rsp::input_port* port, int id) : port_editor(port, id), port(port)
{
	auto* default_port = &port->get_default_port();

	default_data_editor = rsp::gui::editor::create(
		default_port->get_data_type_hash(), default_port->get_data_pointer(), port->get_name(), [=]() {
			default_port->update_timestamp();
			default_port->push();
		});
	default_data_editor->set_maximum_width(200);
}

inline auto input_port_editor::get_port() const -> input_port*
{
	return port;
}

inline void input_port_editor::draw(rsp::gui::widget* override_widget)
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, color);

	float const begin_y = ImGui::GetCursorPosY();

	if(!enabled)
		imnodes::BeginStaticAttribute(id);
	else if(port->is_connected())
		imnodes::BeginInputAttribute(id, imnodes::PinShape_QuadFilled);
	else
		imnodes::BeginInputAttribute(id, imnodes::PinShape_Quad);

	if(override_widget != nullptr)
	{
		override_widget->draw();
	}
	else
	{
		if(!port->is_connected())
		{
			default_data_editor->draw();
		}
		else
		{
			data_viewer->update_data_pointer(port->get_data_pointer());
			data_viewer->draw();
		}

		if(stable_height)
		{
			float current_height = ImGui::GetCursorPosY() - begin_y;
			float max_height = std::max(data_viewer->get_last_size().y, default_data_editor->get_last_size().y);
			if(current_height < max_height)
				ImGui::Dummy(ImVec2(10, max_height - current_height));
		}
	}

	if(!enabled)
		imnodes::EndStaticAttribute();
	else
		imnodes::EndInputAttribute();

	imnodes::PopColorStyle();
}

inline output_port_editor::output_port_editor(rsp::output_port* port, int id) : port_editor(port, id), port(port)
{
}

inline auto output_port_editor::get_port() const -> output_port*
{
	return port;
}

inline void output_port_editor::draw(rsp::gui::widget* override_widget)
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, color);

	if(!enabled)
		imnodes::BeginStaticAttribute(id);
	else if(port->is_connected())
		imnodes::BeginOutputAttribute(id, imnodes::PinShape_TriangleFilled);
	else
		imnodes::BeginOutputAttribute(id, imnodes::PinShape_Triangle);

	if(override_widget != nullptr)
		override_widget->draw();
	else
		data_viewer->draw();

	if(!enabled)
		imnodes::EndStaticAttribute();
	else
		imnodes::EndOutputAttribute();

	imnodes::PopColorStyle();
}

inline auto create_port_editor(rsp::port* port, int id) -> std::unique_ptr<port_editor>
{
	if(auto* input_port = dynamic_cast<rsp::input_port*>(port); input_port != nullptr)
		return std::make_unique<input_port_editor>(input_port, id);
	else if(auto* output_port = dynamic_cast<rsp::output_port*>(port); output_port != nullptr)
		return std::make_unique<output_port_editor>(output_port, id);
	return nullptr;
}

} // namespace rsp::gui::impl