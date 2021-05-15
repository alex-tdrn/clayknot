#pragma once
#include "rsp/base/Port.hpp"
#include "rsp/gui/widgets/Editor.hpp"
#include "rsp/gui/widgets/Viewer.hpp"
#include "rsp/util/ColorRGBA.hpp"

#include <imnodes.h>
#include <memory>

namespace rsp::gui::impl
{
class PortEditor
{
public:
	PortEditor() = delete;
	PortEditor(Port* port, int id);
	PortEditor(PortEditor const&) = delete;
	PortEditor(PortEditor&&) noexcept = delete;
	auto operator=(PortEditor const&) -> PortEditor& = delete;
	auto operator=(PortEditor&&) noexcept -> PortEditor& = delete;
	virtual ~PortEditor() = default;

	auto getID() const -> int;
	auto getColor() const -> std::uint32_t;
	void setEnabled(bool enabled);
	void setStableHeight(bool stableHeight);
	virtual auto getPort() const -> Port* = 0;
	virtual void draw(Widget* overrideWidget = nullptr) = 0;

protected:
	int id = -1; // NOLINT
	std::uint32_t color; // NOLINT
	std::unique_ptr<Viewer> dataViewer; // NOLINT
	bool enabled = true; // NOLINT
	bool stableHeight = false; // NOLINT
};

inline PortEditor::PortEditor(Port* port, int id) : id(id)
{
	dataViewer = Viewer::create(port->getDataTypeHash(), port->getDataPointer(), port->getName());
	dataViewer->setMaximumWidth(200);
	color = ColorRGBA(ColorRGB::createRandom(port->getDataTypeHash()), 1.0f).packed();
}

inline auto PortEditor::getID() const -> int
{
	return id;
}

inline auto PortEditor::getColor() const -> std::uint32_t
{
	return color;
}

inline void PortEditor::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

inline void PortEditor::setStableHeight(bool stableHeight)
{
	this->stableHeight = stableHeight;
}

class InputPortEditor final : public PortEditor
{
public:
	InputPortEditor() = delete;
	InputPortEditor(InputPort* port, int id);
	InputPortEditor(InputPortEditor const&) = delete;
	InputPortEditor(InputPortEditor&&) noexcept = delete;
	auto operator=(InputPortEditor const&) -> InputPortEditor& = delete;
	auto operator=(InputPortEditor&&) noexcept -> InputPortEditor& = delete;
	~InputPortEditor() final = default;

	auto getPort() const -> InputPort* final;
	void draw(Widget* overrideWidget = nullptr) final;

private:
	InputPort* port = nullptr;
	std::unique_ptr<Editor> defaultDataEditor;
};

inline InputPortEditor::InputPortEditor(InputPort* port, int id) : PortEditor(port, id), port(port)
{
	auto* defaultPort = &port->getDefaultPort();

	defaultDataEditor =
		Editor::create(defaultPort->getDataTypeHash(), defaultPort->getDataPointer(), port->getName(), [=]() {
			defaultPort->updateTimestamp();
			defaultPort->push();
		});
	defaultDataEditor->setMaximumWidth(200);
}

inline auto InputPortEditor::getPort() const -> InputPort*
{
	return port;
}

inline void InputPortEditor::draw(Widget* overrideWidget)
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, color);

	float const beginY = ImGui::GetCursorPosY();

	if(!enabled)
		imnodes::BeginStaticAttribute(id);
	else if(port->isConnected())
		imnodes::BeginInputAttribute(id, imnodes::PinShape_QuadFilled);
	else
		imnodes::BeginInputAttribute(id, imnodes::PinShape_Quad);

	if(overrideWidget != nullptr)
	{
		overrideWidget->draw();
	}
	else
	{
		if(!port->isConnected())
		{
			defaultDataEditor->draw();
		}
		else
		{
			dataViewer->updateDataPointer(port->getDataPointer());
			dataViewer->draw();
		}

		if(stableHeight)
		{
			float currentHeight = ImGui::GetCursorPosY() - beginY;
			float maxHeight = std::max(dataViewer->getLastSize().y, defaultDataEditor->getLastSize().y);
			if(currentHeight < maxHeight)
				ImGui::Dummy(ImVec2(10, maxHeight - currentHeight));
		}
	}

	if(!enabled)
		imnodes::EndStaticAttribute();
	else
		imnodes::EndInputAttribute();

	imnodes::PopColorStyle();
}

class OutputPortEditor final : public PortEditor
{
public:
	OutputPortEditor() = delete;
	OutputPortEditor(OutputPort* port, int id);
	OutputPortEditor(OutputPortEditor const&) = delete;
	OutputPortEditor(OutputPortEditor&&) noexcept = delete;
	auto operator=(OutputPortEditor const&) -> OutputPortEditor& = delete;
	auto operator=(OutputPortEditor&&) noexcept -> OutputPortEditor& = delete;
	~OutputPortEditor() final = default;

	auto getPort() const -> OutputPort* final;
	void draw(Widget* overrideWidget = nullptr) final;

private:
	OutputPort* port = nullptr;
};

inline OutputPortEditor::OutputPortEditor(OutputPort* port, int id) : PortEditor(port, id), port(port)
{
}

inline auto OutputPortEditor::getPort() const -> OutputPort*
{
	return port;
}

inline void OutputPortEditor::draw(Widget* overrideWidget)
{
	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, color);

	if(!enabled)
		imnodes::BeginStaticAttribute(id);
	else if(port->isConnected())
		imnodes::BeginOutputAttribute(id, imnodes::PinShape_TriangleFilled);
	else
		imnodes::BeginOutputAttribute(id, imnodes::PinShape_Triangle);

	if(overrideWidget != nullptr)
		overrideWidget->draw();
	else
		dataViewer->draw();

	if(!enabled)
		imnodes::EndStaticAttribute();
	else
		imnodes::EndOutputAttribute();

	imnodes::PopColorStyle();
}

inline auto createPortEditor(Port* port, int id) -> std::unique_ptr<PortEditor>
{
	if(auto* inputPort = dynamic_cast<InputPort*>(port); inputPort != nullptr)
		return std::make_unique<InputPortEditor>(inputPort, id);
	else if(auto* outputPort = dynamic_cast<OutputPort*>(port); outputPort != nullptr)
		return std::make_unique<OutputPortEditor>(outputPort, id);
	return nullptr;
}

} // namespace rsp::gui::impl