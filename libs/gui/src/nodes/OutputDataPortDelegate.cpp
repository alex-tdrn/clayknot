#include "rsp/gui/nodes/OutputDataPortDelegate.h"
#include "rsp/base/ColorRGB.hpp"
#include "rsp/base/port/OutputDataPort.hpp"
#include "rsp/gui/widgets/Viewer.hpp"

#include <imgui.h>

namespace rsp::gui
{
template <typename T>
class Delegate : public OutputDataPortDelegate
{
	rsp::OutputDataPort<T> const* port;
	rsp::gui::Viewer<T> viewer;
	std::string id = "##OutputPort";

public:
	Delegate(rsp::OutputDataPort<T> const* port, std::string name) : port(port), viewer(port->get(), name)
	{
		id += port->getName();
		viewer.setMaximumWidth(200);
	}

	void draw() const
	{
		viewer.draw();
	}
};

std::unique_ptr<OutputDataPortDelegate> OutputDataPortDelegate::create(rsp::DataPort* port, std::string name)
{
	auto colorPort = dynamic_cast<rsp::OutputDataPort<rsp::ColorRGB> const*>(port);
	if(colorPort)
		return std::make_unique<Delegate<rsp::ColorRGB>>(colorPort, name);

	auto floatPort = dynamic_cast<rsp::OutputDataPort<float> const*>(port);
	if(floatPort)
		return std::make_unique<Delegate<float>>(floatPort, name);

	assert(false);
	return nullptr;
}

} // namespace rsp::gui