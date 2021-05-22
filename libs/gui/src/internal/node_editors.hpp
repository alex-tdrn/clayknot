#pragma once
#include "clk/base/constant_node.hpp"
#include "clk/base/node.hpp"
#include "clk/util/color_rgba.hpp"
#include "port_editors.hpp"
#include "widget_cache.hpp"


#include <imgui.h>
#include <imnodes.h>

namespace clk::gui::impl
{
class node_editor
{
public:
	node_editor() = delete;
	node_editor(clk::node* node, int id, widget_cache<clk::port, port_editor>* portCache,
		std::optional<std::function<bool()>>& modification_callback);
	node_editor(node_editor const&) = delete;
	node_editor(node_editor&&) noexcept = delete;
	auto operator=(node_editor const&) -> node_editor& = delete;
	auto operator=(node_editor&&) noexcept -> node_editor& = delete;
	virtual ~node_editor() = default;

	auto get_id() const -> int;
	auto get_node() const -> clk::node*;
	void set_highlighted(bool highlighted);
	void draw();

protected:
	std::optional<std::function<bool()>>& modification_callback;
	widget_cache<clk::port, port_editor>* port_cache = nullptr; // NOLINT
	clk::node* node = nullptr; // NOLINT
	int id = -1; // NOLINT
	bool first_draw = true; // NOLINT
	float title_width = 0; // NOLINT
	float contents_width = 0; // NOLINT
	bool highlighted = false; // NOLINT

	virtual void draw_title_bar();
	virtual void draw_input_ports();
	virtual void draw_output_ports();
};

class constant_node_editor final : public node_editor
{
public:
	constant_node_editor() = delete;
	constant_node_editor(clk::constant_node* node, int id, widget_cache<clk::port, port_editor>* port_cache,
		std::optional<std::function<bool()>>& modification_callback);
	constant_node_editor(constant_node_editor const&) = delete;
	constant_node_editor(constant_node_editor&&) noexcept = delete;
	auto operator=(constant_node_editor const&) -> constant_node_editor& = delete;
	auto operator=(constant_node_editor&&) noexcept -> constant_node_editor& = delete;
	~constant_node_editor() final = default;

private:
	clk::constant_node* node;
	std::unordered_map<clk::output_port*, std::unique_ptr<clk::gui::editor>> constant_editors;

	void draw_output_ports() final;
};

inline node_editor::node_editor(clk::node* node, int id, widget_cache<clk::port, port_editor>* port_cache,
	std::optional<std::function<bool()>>& modification_callback)
	: modification_callback(modification_callback), port_cache(port_cache), node(node), id(id)
{
}

inline auto node_editor::get_id() const -> int
{
	return id;
}

inline auto node_editor::get_node() const -> clk::node*
{
	return node;
}

inline void node_editor::set_highlighted(bool highlighted)
{
	this->highlighted = highlighted;
}

inline void node_editor::draw()
{
	if(highlighted)
		imnodes::PushColorStyle(imnodes::ColorStyle_NodeOutline, color_rgba{1.0f}.packed());

	imnodes::BeginNode(id);

	imnodes::BeginNodeTitleBar();
	ImGui::BeginGroup();
	if(!first_draw && title_width < contents_width)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (contents_width - title_width) / 2);
	draw_title_bar();
	ImGui::EndGroup();
	if(first_draw)
		title_width = ImGui::GetItemRectSize().x;
	imnodes::EndNodeTitleBar();

	ImGui::BeginGroup();
	draw_input_ports();
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	draw_output_ports();
	ImGui::EndGroup();

	imnodes::EndNode();
	contents_width = ImGui::GetItemRectSize().x;

	if(highlighted)
		imnodes::PopColorStyle();

	first_draw = false;
}

inline void node_editor::draw_title_bar()
{
	if(!this->node->get_input_ports().empty())
	{
		if(ImGui::SmallButton("Pull"))
			this->node->pull();
		ImGui::SameLine();
	}

	ImGui::Text("%s", this->node->get_name().data());

	if(!this->node->get_output_ports().empty())
	{
		ImGui::SameLine();
		if(ImGui::SmallButton("Push"))
			this->node->push();
	}
}

inline void node_editor::draw_input_ports()
{
	for(auto& port : this->node->get_input_ports())
		port_cache->get_widget(port).draw();
}

inline void node_editor::draw_output_ports()
{
	for(auto& port : this->node->get_output_ports())
		port_cache->get_widget(port).draw();
}

inline constant_node_editor::constant_node_editor(clk::constant_node* node, int id,
	widget_cache<clk::port, port_editor>* portCache, std::optional<std::function<bool()>>& modificationCallback)
	: node_editor(node, id, portCache, modificationCallback), node(node)
{
}

inline void constant_node_editor::draw_output_ports()
{
	for(auto* port : this->node->get_output_ports())
	{
		ImGui::PushID(port);
		if(ImGui::SmallButton("-"))
		{
			if(!modification_callback.has_value())
			{
				modification_callback = [&]() {
					constant_editors.erase(port);
					this->node->remove_port(port);
					return true;
				};
			}
		}
		ImGui::PopID();
		ImGui::SameLine();
		if(constant_editors.find(port) == constant_editors.end())
		{
			constant_editors[port] =
				clk::gui::editor::create(port->get_data_type_hash(), port->get_data_pointer(), port->get_name(), [=]() {
					port->update_timestamp();
					port->push();
				});
			constant_editors[port]->set_maximum_width(200);
		}

		port_cache->get_widget(port).draw(constant_editors[port].get());
	}

	if(ImGui::SmallButton("+"))
	{
		if(!modification_callback.has_value())
		{
			modification_callback = [&]() {
				bool done = false;

				ImGui::OpenPopup("Add Constant Port Menu");
				if(ImGui::BeginPopup("Add Constant Port Menu"))
				{
					std::unique_ptr<output_port> constantPort = nullptr;

					if(ImGui::MenuItem("int"))
						constantPort = std::make_unique<output_port_of<int>>("Constant");
					else if(ImGui::MenuItem("float"))
						constantPort = std::make_unique<output_port_of<float>>("Constant");
					else if(ImGui::MenuItem("color"))
						constantPort = std::make_unique<output_port_of<color_rgb>>("Constant");

					if(constantPort != nullptr)
					{
						this->node->add_port(std::move(constantPort));
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

inline auto create_node_editor(clk::node* node, int id, widget_cache<clk::port, port_editor>* portCache,
	std::optional<std::function<bool()>>& modificationCallback) -> std::unique_ptr<node_editor>
{
	if(auto* constantNode = dynamic_cast<clk::constant_node*>(node))
		return std::make_unique<constant_node_editor>(constantNode, id, portCache, modificationCallback);
	else
		return std::make_unique<node_editor>(node, id, portCache, modificationCallback);
}

} // namespace clk::gui::impl