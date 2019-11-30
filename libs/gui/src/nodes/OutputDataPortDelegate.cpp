#include "rshp/gui/nodes/OutputDataPortDelegate.h"
#include "rshp/base/ColorRGB.hpp"
#include "rshp/base/port/OutputDataPort.hpp"

#include <imgui.h>

namespace rshp::gui
{
template <typename T>
class Delegate : public OutputDataPortDelegate
{
	rshp::base::OutputDataPort<T> const* port;
	std::string id = "##OutputPort";

public:
	Delegate(rshp::base::OutputDataPort<T> const* port) : port(port)
	{
		id += port->getName();
	}

	void draw() const;
};

template <>
void Delegate<rshp::base::ColorRGB>::draw() const
{
	auto output = port->get();
	ImGui::ColorButton(id.c_str(), {output.r(), output.g(), output.b(), 1},
		ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(32, 32));
}

template <>
void Delegate<float>::draw() const
{
	auto output = port->get();
	ImGui::Text("%.2f", output);
}

std::unique_ptr<OutputDataPortDelegate> OutputDataPortDelegate::create(rshp::base::DataPort* port)
{
	auto colorPort = dynamic_cast<rshp::base::OutputDataPort<rshp::base::ColorRGB> const*>(port);
	if(colorPort)
		return std::make_unique<Delegate<rshp::base::ColorRGB>>(colorPort);

	auto floatPort = dynamic_cast<rshp::base::OutputDataPort<float> const*>(port);
	if(floatPort)
		return std::make_unique<Delegate<float>>(floatPort);

	assert(false);
	return nullptr;
}

} // namespace rshp::gui