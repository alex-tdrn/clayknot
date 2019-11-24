#include "rshp/gui/nodes/OutputDataPortDelegate.h"
#include "rshp/base/port/OutputDataPort.hpp"

#include <glm/glm.hpp>
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
void Delegate<glm::vec3>::draw() const
{
	auto output = port->get();
	ImGui::ColorButton(id.c_str(), {output.r, output.g, output.b, 1},
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
	auto colorPort = dynamic_cast<rshp::base::OutputDataPort<glm::vec3> const*>(port);
	if(colorPort)
		return std::make_unique<Delegate<glm::vec3>>(colorPort);

	auto floatPort = dynamic_cast<rshp::base::OutputDataPort<float> const*>(port);
	if(floatPort)
		return std::make_unique<Delegate<float>>(floatPort);

	assert(false);
	return nullptr;
}

} // namespace rshp::gui