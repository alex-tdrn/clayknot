#pragma once
#include "rsp/base/Port.hpp"
#include "rsp/gui/widgets/Viewer.hpp"
#include "rsp/util/ColorRGBA.hpp"

#include <imnodes.h>
#include <memory>

namespace rsp::gui::impl
{
class PortViewer
{
public:
	PortViewer() = delete;
	PortViewer(Port const* port, int id);
	PortViewer(PortViewer const&) = delete;
	PortViewer(PortViewer&&) noexcept = delete;
	auto operator=(PortViewer const&) -> PortViewer& = delete;
	auto operator=(PortViewer&&) noexcept -> PortViewer& = delete;
	virtual ~PortViewer() = default;

	auto getID() const -> int;
	virtual auto getPort() const -> Port const* = 0;
	virtual void draw() = 0;

protected:
	int id = -1; // NOLINT
	std::uint32_t color; // NOLINT
	std::unique_ptr<Viewer> dataViewer; // NOLINT
};

inline PortViewer::PortViewer(Port const* port, int id) : id(id)
{
	dataViewer = Viewer::create(port->getDataTypeHash(), port->getDataPointer(), port->getName());
	dataViewer->setMaximumWidth(200);
	color = ColorRGBA(ColorRGB::createRandom(port->getDataTypeHash()), 1.0f).packed();
}

inline auto PortViewer::getID() const -> int
{
	return id;
}

class InputPortViewer final : public PortViewer
{
public:
	InputPortViewer() = delete;
	InputPortViewer(InputPort const* port, int id);
	InputPortViewer(InputPortViewer const&) = delete;
	InputPortViewer(InputPortViewer&&) noexcept = delete;
	auto operator=(InputPortViewer const&) -> InputPortViewer& = delete;
	auto operator=(InputPortViewer&&) noexcept -> InputPortViewer& = delete;
	~InputPortViewer() final = default;

	auto getPort() const -> InputPort const* final;
	void draw() final;

private:
	InputPort const* port = nullptr;
};

inline InputPortViewer::InputPortViewer(InputPort const* port, int id) : PortViewer(port, id), port(port)
{
}

inline auto InputPortViewer::getPort() const -> InputPort const*
{
	return port;
}

inline void InputPortViewer::draw()
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, color);
	imnodes::PushColorStyle(imnodes::ColorStyle_PinHovered, color);

	if(port->isConnected())
		imnodes::BeginInputAttribute(id, imnodes::PinShape_QuadFilled);
	else
		imnodes::BeginInputAttribute(id, imnodes::PinShape_Quad);

	dataViewer->updateDataPointer(port->getDataPointer());
	dataViewer->draw();

	imnodes::EndInputAttribute();

	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
}

class OutputPortViewer final : public PortViewer
{
public:
	OutputPortViewer() = delete;
	OutputPortViewer(OutputPort const* port, int id);
	OutputPortViewer(OutputPortViewer const&) = delete;
	OutputPortViewer(OutputPortViewer&&) noexcept = delete;
	auto operator=(OutputPortViewer const&) -> OutputPortViewer& = delete;
	auto operator=(OutputPortViewer&&) noexcept -> OutputPortViewer& = delete;
	~OutputPortViewer() final = default;

	auto getPort() const -> OutputPort const* final;
	void draw() final;

private:
	OutputPort const* port = nullptr;
};

inline OutputPortViewer::OutputPortViewer(OutputPort const* port, int id) : PortViewer(port, id), port(port)
{
}

inline auto OutputPortViewer::getPort() const -> OutputPort const*
{
	return port;
}

inline void OutputPortViewer::draw()
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, color);
	imnodes::PushColorStyle(imnodes::ColorStyle_PinHovered, color);

	if(port->isConnected())
		imnodes::BeginOutputAttribute(id, imnodes::PinShape_TriangleFilled);
	else
		imnodes::BeginOutputAttribute(id, imnodes::PinShape_Triangle);

	dataViewer->draw();

	imnodes::EndOutputAttribute();

	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
}

inline auto createPortViewer(Port const* port, int id) -> std::unique_ptr<PortViewer>
{
	if(auto const* inputPort = dynamic_cast<InputPort const*>(port); inputPort != nullptr)
		return std::make_unique<InputPortViewer>(inputPort, id);
	else if(auto const* outputPort = dynamic_cast<OutputPort const*>(port); outputPort != nullptr)
		return std::make_unique<OutputPortViewer>(outputPort, id);
	return nullptr;
}

} // namespace rsp::gui::impl