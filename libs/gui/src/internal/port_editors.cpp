#include "port_editors.hpp"
#include "clk/base/port.hpp"
#include "clk/gui/widgets/data_reader.hpp"
#include "clk/gui/widgets/data_writer.hpp"
#include "clk/gui/widgets/widget.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"

#include <algorithm>
#include <imgui.h>
#include <imnodes.h>

namespace clk::gui::impl
{
port_editor::port_editor(clk::port* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
	: _id(id)
	, _color(color_rgba(color_rgb::create_random(port->data_type_hash()), 1.0f).packed())
	, _data_viewer(widget_factory.create(data_reader<void>{[=]() {
		return port->data_pointer();
	}},
		  port->data_type_hash(), port->name()))
	, _draw_port_widgets(draw_port_widgets)
{
	_data_viewer->set_maximum_width(200);
}

auto port_editor::id() const -> int
{
	return _id;
}

auto port_editor::color() const -> std::uint32_t
{
	return _color;
}

void port_editor::set_enabled(bool enabled)
{
	_enabled = enabled;
}

void port_editor::set_stable_height(bool stable_height)
{
	_stable_height = stable_height;
}

auto port_editor::position() const -> glm::vec2 const&
{
	return _position;
}

input_editor::input_editor(
	clk::input* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
	: port_editor(port, id, widget_factory, draw_port_widgets), _port(port)
{
	auto* default_port = &port->default_port();

	_default_data_editor = widget_factory.create(clk::gui::data_writer<void>{[=]() {
																				 return default_port->data_pointer();
																			 },
													 [=]() {
														 default_port->update_timestamp();
														 default_port->push();
													 }},
		default_port->data_type_hash(), port->name());
	_default_data_editor->set_maximum_width(200);
}

auto input_editor::port() const -> input*
{
	return _port;
}

void input_editor::draw(clk::gui::widget* override_widget)
{
	if(_port->is_faulty())
	{
		const float t = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(
			std::chrono::steady_clock::now().time_since_epoch())
							.count();
		const float f = (std::cos(t * 20.0f) + 1.0f) / 2.0f;
		auto c1 = color_rgba{1.0f, 0.0f, 0.0f, 1.0f};
		auto c2 = color_rgba{1.0f};
		auto error_color = (f * c1 + (1.0f - f) * c2).packed();

		ImNodes::PushColorStyle(ImNodesCol_Pin, error_color);
		ImNodes::PushColorStyle(ImNodesCol_PinHovered, error_color);
	}
	else
	{
		ImNodes::PushColorStyle(ImNodesCol_Pin, _color);
	}

	float const begin_y = ImGui::GetCursorPosY();

	if(!_enabled)
		ImNodes::BeginStaticAttribute(_id);
	else
		ImNodes::BeginInputAttribute(_id, ImNodesPinShape_QuadFilled);

	if(_draw_port_widgets)
	{
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
				_data_viewer->draw();
			}

			if(_stable_height)
			{
				float current_height = ImGui::GetCursorPosY() - begin_y;
				float max_height = std::max(_data_viewer->last_size().y, _default_data_editor->last_size().y);
				if(current_height < max_height)
					ImGui::Dummy(ImVec2(10, max_height - current_height));
			}
		}
	}
	else
	{
		ImGui::Dummy(ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()));
	}

	if(!_enabled)
		ImNodes::EndStaticAttribute();
	else
		ImNodes::EndInputAttribute();

	auto rect_min = glm::vec2(ImGui::GetItemRectMin());
	auto rect_max = glm::vec2(ImGui::GetItemRectMax());
	_position.y = (rect_min.y + rect_max.y) / 2;
	_position.x = rect_min.x;

	ImNodes::PopColorStyle();
	if(_port->is_faulty())
	{
		ImNodes::PopColorStyle();
	}
}

output_editor::output_editor(
	clk::output* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
	: port_editor(port, id, widget_factory, draw_port_widgets), _port(port)
{
}

auto output_editor::port() const -> output*
{
	return _port;
}

void output_editor::draw(clk::gui::widget* override_widget)
{
	if(_port->is_faulty())
	{
		const float t = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(
			std::chrono::steady_clock::now().time_since_epoch())
							.count();
		const float f = (std::cos(t * 20.0f) + 1.0f) / 2.0f;
		auto c1 = color_rgba{1.0f, 0.0f, 0.0f, 1.0f};
		auto c2 = color_rgba{1.0f};
		auto error_color = (f * c1 + (1.0f - f) * c2).packed();

		ImNodes::PushColorStyle(ImNodesCol_Pin, error_color);
		ImNodes::PushColorStyle(ImNodesCol_PinHovered, error_color);
	}
	else
	{
		ImNodes::PushColorStyle(ImNodesCol_Pin, _color);
	}

	if(!_enabled)
		ImNodes::BeginStaticAttribute(_id);
	else
		ImNodes::BeginOutputAttribute(_id, ImNodesPinShape_TriangleFilled);

	if(_draw_port_widgets)
	{
		if(override_widget != nullptr)
			override_widget->draw();
		else
			_data_viewer->draw();
	}
	else
	{
		ImGui::Dummy(ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()));
	}

	if(!_enabled)
		ImNodes::EndStaticAttribute();
	else
		ImNodes::EndOutputAttribute();

	auto rect_min = glm::vec2(ImGui::GetItemRectMin());
	auto rect_max = glm::vec2(ImGui::GetItemRectMax());
	_position.y = (rect_min.y + rect_max.y) / 2;
	_position.x = rect_max.x;

	ImNodes::PopColorStyle();
	if(_port->is_faulty())
	{
		ImNodes::PopColorStyle();
	}
}

auto create_port_editor(clk::port* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
	-> std::unique_ptr<port_editor>
{
	if(auto* input = dynamic_cast<clk::input*>(port); input != nullptr)
		return std::make_unique<input_editor>(input, id, widget_factory, draw_port_widgets);
	else if(auto* output = dynamic_cast<clk::output*>(port); output != nullptr)
		return std::make_unique<output_editor>(output, id, widget_factory, draw_port_widgets);
	return nullptr;
}

} // namespace clk::gui::impl