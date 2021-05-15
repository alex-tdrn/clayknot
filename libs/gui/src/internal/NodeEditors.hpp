#pragma once
#include "PortEditors.hpp"
#include "WidgetCache.hpp"
#include "rsp/base/ConstantNode.hpp"
#include "rsp/base/Node.hpp"
#include "rsp/util/ColorRGBA.hpp"

#include <imgui.h>
#include <imnodes.h>

namespace rsp::gui::impl
{
class NodeEditor
{
public:
	NodeEditor() = delete;
	NodeEditor(Node* node, int id, WidgetCache<Port, PortEditor>* portCache,
		std::optional<std::function<bool()>>& modificationCallback);
	NodeEditor(NodeEditor const&) = delete;
	NodeEditor(NodeEditor&&) noexcept = delete;
	auto operator=(NodeEditor const&) -> NodeEditor& = delete;
	auto operator=(NodeEditor&&) noexcept -> NodeEditor& = delete;
	virtual ~NodeEditor() = default;

	auto getID() const -> int;
	auto getNode() const -> Node*;
	void setHighlighted(bool highlighted);
	void draw();

protected:
	std::optional<std::function<bool()>>& modificationCallback;
	WidgetCache<Port, PortEditor>* portCache = nullptr; // NOLINT
	Node* node = nullptr; // NOLINT
	int id = -1; // NOLINT
	bool firstDraw = true; // NOLINT
	float titleWidth = 0; // NOLINT
	float contentsWidth = 0; // NOLINT
	bool highlighted = false; // NOLINT

	virtual void drawTitleBar();
	virtual void drawInputPorts();
	virtual void drawOutputPorts();
};

inline NodeEditor::NodeEditor(Node* node, int id, WidgetCache<Port, PortEditor>* portCache,
	std::optional<std::function<bool()>>& modificationCallback)
	: modificationCallback(modificationCallback), portCache(portCache), node(node), id(id)
{
}

inline auto NodeEditor::getID() const -> int
{
	return id;
}

inline auto NodeEditor::getNode() const -> Node*
{
	return node;
}

inline void NodeEditor::setHighlighted(bool highlighted)
{
	this->highlighted = highlighted;
}

inline void NodeEditor::draw()
{
	if(highlighted)
		imnodes::PushColorStyle(imnodes::ColorStyle_NodeOutline, ColorRGBA{1.0f}.packed());

	imnodes::BeginNode(id);

	imnodes::BeginNodeTitleBar();
	ImGui::BeginGroup();
	if(!firstDraw && titleWidth < contentsWidth)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (contentsWidth - titleWidth) / 2);
	drawTitleBar();
	ImGui::EndGroup();
	if(firstDraw)
		titleWidth = ImGui::GetItemRectSize().x;
	imnodes::EndNodeTitleBar();

	ImGui::BeginGroup();
	drawInputPorts();
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	drawOutputPorts();
	ImGui::EndGroup();

	imnodes::EndNode();
	contentsWidth = ImGui::GetItemRectSize().x;

	if(highlighted)
		imnodes::PopColorStyle();

	firstDraw = false;
}

inline void NodeEditor::drawTitleBar()
{
	if(!node->getInputPorts().empty())
	{
		if(ImGui::SmallButton("Pull"))
			node->pull();
		ImGui::SameLine();
	}

	ImGui::Text("%s", node->getName().data());

	if(!node->getOutputPorts().empty())
	{
		ImGui::SameLine();
		if(ImGui::SmallButton("Push"))
			node->push();
	}
}

inline void NodeEditor::drawInputPorts()
{
	for(auto& port : node->getInputPorts())
		portCache->getWidget(port).draw();
}

inline void NodeEditor::drawOutputPorts()
{
	for(auto& port : node->getOutputPorts())
		portCache->getWidget(port).draw();
}

class ConstantNodeEditor final : public NodeEditor
{
public:
	ConstantNodeEditor() = delete;
	ConstantNodeEditor(ConstantNode* node, int id, WidgetCache<Port, PortEditor>* portCache,
		std::optional<std::function<bool()>>& modificationCallback);
	ConstantNodeEditor(ConstantNodeEditor const&) = delete;
	ConstantNodeEditor(ConstantNodeEditor&&) noexcept = delete;
	auto operator=(ConstantNodeEditor const&) -> ConstantNodeEditor& = delete;
	auto operator=(ConstantNodeEditor&&) noexcept -> ConstantNodeEditor& = delete;
	~ConstantNodeEditor() final = default;

private:
	ConstantNode* node;
	std::unordered_map<OutputPort*, std::unique_ptr<Editor>> constantEditors;

	void drawOutputPorts() final;
};

inline ConstantNodeEditor::ConstantNodeEditor(ConstantNode* node, int id, WidgetCache<Port, PortEditor>* portCache,
	std::optional<std::function<bool()>>& modificationCallback)
	: NodeEditor(node, id, portCache, modificationCallback), node(node)
{
}

inline void ConstantNodeEditor::drawOutputPorts()
{
	for(auto* port : node->getOutputPorts())
	{
		ImGui::PushID(port);
		if(ImGui::SmallButton("-"))
		{
			if(!modificationCallback.has_value())
			{
				modificationCallback = [&]() {
					constantEditors.erase(port);
					node->removePort(port);
					return true;
				};
			}
		}
		ImGui::PopID();
		ImGui::SameLine();
		if(constantEditors.find(port) == constantEditors.end())
		{
			constantEditors[port] =
				Editor::create(port->getDataTypeHash(), port->getDataPointer(), port->getName(), [=]() {
					port->updateTimestamp();
					port->push();
				});
			constantEditors[port]->setMaximumWidth(200);
		}

		portCache->getWidget(port).draw(constantEditors[port].get());
	}

	if(ImGui::SmallButton("+"))
	{
		if(!modificationCallback.has_value())
		{
			modificationCallback = [&]() {
				bool done = false;

				ImGui::OpenPopup("Add Constant Port Menu");
				if(ImGui::BeginPopup("Add Constant Port Menu"))
				{
					std::unique_ptr<OutputPort> constantPort = nullptr;

					if(ImGui::MenuItem("int"))
						constantPort = std::make_unique<OutputPortOf<int>>("Constant");
					else if(ImGui::MenuItem("float"))
						constantPort = std::make_unique<OutputPortOf<float>>("Constant");
					else if(ImGui::MenuItem("color"))
						constantPort = std::make_unique<OutputPortOf<ColorRGB>>("Constant");

					if(constantPort != nullptr)
					{
						node->addPort(std::move(constantPort));
						done = true;
					}
					ImGui::EndPopup();
				}
				else
				{
					done = true;
				}
				return done;
			};
		}
	}
}

inline auto createNodeEditor(Node* node, int id, WidgetCache<Port, PortEditor>* portCache,
	std::optional<std::function<bool()>>& modificationCallback) -> std::unique_ptr<NodeEditor>
{
	if(auto* constantNode = dynamic_cast<ConstantNode*>(node))
		return std::make_unique<ConstantNodeEditor>(constantNode, id, portCache, modificationCallback);
	else
		return std::make_unique<NodeEditor>(node, id, portCache, modificationCallback);
}

} // namespace rsp::gui::impl