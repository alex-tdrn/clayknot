#include "OutputDataPinDelegate.h"
#include "rshp/base/OutputDataPort.hpp"

#include <imgui.h>
#include <glm/glm.hpp>

template<typename T>
class Delegate : public OutputDataPinDelegate
{
	rshp::base::OutputDataPort<T> const* port;
	std::string id = "##OutputPin";

public:
	Delegate(rshp::base::OutputDataPort<T> const* port)
		: port(port)
	{
		id += port->getName();
	}

	void draw() const;

};

template<>
void Delegate<glm::vec3>::draw() const
{
	auto output = port->get();
	ImGui::ColorButton(id.c_str(), {output.r, output.g, output.b, 1}, 
		ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(32, 32));
}

template<>
void Delegate<float>::draw() const
{
	auto output = port->get();
	ImGui::Text("%.2f", output);
}

std::unique_ptr<OutputDataPinDelegate> OutputDataPinDelegate::create(rshp::base::AbstractDataPort* port)
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
