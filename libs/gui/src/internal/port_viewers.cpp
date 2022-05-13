#include "port_viewers.hpp"
#include "clk/base/port.hpp"
#include "clk/gui/widgets/data_reader.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"

#include <imgui.h>
#include <imnodes.h>

namespace clk::gui::impl
{

port_viewer::port_viewer(
	clk::port const* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
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

auto port_viewer::id() const -> int
{
	return _id;
}

auto port_viewer::position() const -> glm::vec2 const&
{
	return _position;
}

input_viewer::input_viewer(
	clk::input const* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
	: port_viewer(port, id, widget_factory, draw_port_widgets), _port(port)
{
}

auto input_viewer::port() const -> clk::input const*
{
	return _port;
}

void input_viewer::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_Pin, _color);
	ImNodes::PushColorStyle(ImNodesCol_PinHovered, _color);

	ImNodes::BeginInputAttribute(_id, ImNodesPinShape_QuadFilled);

	if(_draw_port_widgets)
	{
		_data_viewer->draw();
	}
	else
	{
		ImGui::Dummy(ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()));
	}

	ImNodes::EndInputAttribute();
	auto rect_min = glm::vec2(ImGui::GetItemRectMin());
	auto rect_max = glm::vec2(ImGui::GetItemRectMax());
	_position.y = (rect_min.y + rect_max.y) / 2;
	_position.x = rect_min.x;
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

output_viewer::output_viewer(
	clk::output const* port, int id, widget_factory const& widget_factory, bool const& draw_port_widgets)
	: port_viewer(port, id, widget_factory, draw_port_widgets), _port(port)
{
}

auto output_viewer::port() const -> output const*
{
	return _port;
}

void output_viewer::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_Pin, _color);
	ImNodes::PushColorStyle(ImNodesCol_PinHovered, _color);

	ImNodes::BeginOutputAttribute(_id, ImNodesPinShape_TriangleFilled);

	if(_draw_port_widgets)
	{
		_data_viewer->draw();
	}
	else
	{
		ImGui::Dummy(ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()));
	}

	ImNodes::EndOutputAttribute();
	auto rect_min = glm::vec2(ImGui::GetItemRectMin());
	auto rect_max = glm::vec2(ImGui::GetItemRectMax());
	_position.y = (rect_min.y + rect_max.y) / 2;
	_position.x = rect_max.x;

	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

auto create_port_viewer(clk::port const* port, int id, widget_factory const& widget_factory,
	bool const& draw_port_widgets) -> std::unique_ptr<port_viewer>
{
	if(auto const* inputPort = dynamic_cast<clk::input const*>(port); inputPort != nullptr)
		return std::make_unique<input_viewer>(inputPort, id, widget_factory, draw_port_widgets);
	else if(auto const* outputPort = dynamic_cast<clk::output const*>(port); outputPort != nullptr)
		return std::make_unique<output_viewer>(outputPort, id, widget_factory, draw_port_widgets);
	return nullptr;
}

} // namespace clk::gui::impl