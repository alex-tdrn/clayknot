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
	node_editor(clk::node* node, int id, widget_cache<clk::port, port_editor>* port_cache,
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
	std::optional<std::function<bool()>>& _modification_callback; // NOLINT
	widget_cache<clk::port, port_editor>* _port_cache = nullptr; // NOLINT
	clk::node* _node = nullptr; // NOLINT
	int _id = -1; // NOLINT
	bool _first_draw = true; // NOLINT
	float _title_width = 0; // NOLINT
	float _contents_width = 0; // NOLINT
	bool _highlighted = false; // NOLINT

	virtual void draw_title_bar();
	virtual void draw_inputs();
	virtual void draw_outputs();
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
	clk::constant_node* _node;
	std::unordered_map<clk::output*, std::unique_ptr<clk::gui::editor>> _constant_editors;

	void draw_outputs() final;
};

inline node_editor::node_editor(clk::node* node, int id, widget_cache<clk::port, port_editor>* port_cache,
	std::optional<std::function<bool()>>& modification_callback)
	: _modification_callback(modification_callback), _port_cache(port_cache), _node(node), _id(id)
{
}

inline auto node_editor::get_id() const -> int
{
	return _id;
}

inline auto node_editor::get_node() const -> clk::node*
{
	return _node;
}

inline void node_editor::set_highlighted(bool highlighted)
{
	_highlighted = highlighted;
}

inline void node_editor::draw()
{
	if(_highlighted)
		imnodes::PushColorStyle(imnodes::ColorStyle_NodeOutline, color_rgba{1.0f}.packed());

	imnodes::BeginNode(_id);

	imnodes::BeginNodeTitleBar();
	ImGui::BeginGroup();
	if(!_first_draw && _title_width < _contents_width)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (_contents_width - _title_width) / 2);
	draw_title_bar();
	ImGui::EndGroup();
	if(_first_draw)
		_title_width = ImGui::GetItemRectSize().x;
	imnodes::EndNodeTitleBar();

	ImGui::BeginGroup();
	draw_inputs();
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	draw_outputs();
	ImGui::EndGroup();

	imnodes::EndNode();
	_contents_width = ImGui::GetItemRectSize().x;

	if(_highlighted)
		imnodes::PopColorStyle();

	_first_draw = false;
}

inline void node_editor::draw_title_bar()
{
	if(!_node->get_inputs().empty())
	{
		if(ImGui::SmallButton("Pull"))
			_node->pull();
		ImGui::SameLine();
	}

	ImGui::Text("%s", _node->get_name().data());

	if(!_node->get_outputs().empty())
	{
		ImGui::SameLine();
		if(ImGui::SmallButton("Push"))
			_node->push();
	}
}

inline void node_editor::draw_inputs()
{
	for(auto* port : _node->get_inputs())
		_port_cache->get_widget(port).draw();
}

inline void node_editor::draw_outputs()
{
	for(auto* port : _node->get_outputs())
		_port_cache->get_widget(port).draw();
}

inline constant_node_editor::constant_node_editor(clk::constant_node* node, int id,
	widget_cache<clk::port, port_editor>* portCache, std::optional<std::function<bool()>>& modificationCallback)
	: node_editor(node, id, portCache, modificationCallback), _node(node)
{
}

inline void constant_node_editor::draw_outputs()
{
	for(auto* port : _node->get_outputs())
	{
		ImGui::PushID(port);
		if(ImGui::SmallButton("-"))
		{
			if(!_modification_callback.has_value())
			{
				_modification_callback = [&]() {
					_constant_editors.erase(port);
					_node->remove_output(port);
					return true;
				};
			}
		}
		ImGui::PopID();
		ImGui::SameLine();
		if(_constant_editors.find(port) == _constant_editors.end())
		{
			_constant_editors[port] =
				clk::gui::editor::create(port->get_data_type_hash(), port->get_data_pointer(), port->get_name(), [=]() {
					port->update_timestamp();
					port->push();
				});
			_constant_editors[port]->set_maximum_width(200);
		}

		_port_cache->get_widget(port).draw(_constant_editors[port].get());
	}

	if(ImGui::SmallButton("+"))
	{
		if(!_modification_callback.has_value())
		{
			_modification_callback = [&]() {
				bool done = false;

				ImGui::OpenPopup("Add Constant Port Menu");
				if(ImGui::BeginPopup("Add Constant Port Menu"))
				{
					std::unique_ptr<output> constantPort = nullptr;

					if(ImGui::MenuItem("int"))
						constantPort = std::make_unique<output_of<int>>("Constant");
					else if(ImGui::MenuItem("float"))
						constantPort = std::make_unique<output_of<float>>("Constant");
					else if(ImGui::MenuItem("color"))
						constantPort = std::make_unique<output_of<color_rgb>>("Constant");

					if(constantPort != nullptr)
					{
						_node->add_output(std::move(constantPort));
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