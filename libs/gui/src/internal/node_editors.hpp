#pragma once
#include "clk/base/constant_node.hpp"
#include "clk/base/node.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
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
		std::optional<std::function<bool()>>& queued_action, bool const& draw_node_titles);
	node_editor(node_editor const&) = delete;
	node_editor(node_editor&&) noexcept = delete;
	auto operator=(node_editor const&) -> node_editor& = delete;
	auto operator=(node_editor&&) noexcept -> node_editor& = delete;
	virtual ~node_editor() = default;

	auto id() const -> int;
	auto node() const -> clk::node*;
	void set_highlighted(bool highlighted);
	void draw();

protected:
	std::optional<std::function<bool()>>& _queued_action; // NOLINT
	widget_cache<clk::port, port_editor>* _port_cache = nullptr; // NOLINT
	clk::node* _node = nullptr; // NOLINT
	int _id = -1; // NOLINT
	bool _first_draw = true; // NOLINT
	float _title_width = 0; // NOLINT
	float _contents_width = 0; // NOLINT
	bool _highlighted = false; // NOLINT
	bool const& _draw_node_titles; // NOLINT

	virtual void draw_title_bar();
	virtual void draw_inputs();
	virtual void draw_outputs();
};

class constant_node_editor final : public node_editor
{
public:
	constant_node_editor() = delete;
	constant_node_editor(clk::constant_node* constant_node, int id, widget_cache<clk::port, port_editor>* port_cache,
		std::optional<std::function<bool()>>& queued_action, widget_factory const& widget_factory,
		bool const& draw_node_titles);
	constant_node_editor(constant_node_editor const&) = delete;
	constant_node_editor(constant_node_editor&&) noexcept = delete;
	auto operator=(constant_node_editor const&) -> constant_node_editor& = delete;
	auto operator=(constant_node_editor&&) noexcept -> constant_node_editor& = delete;
	~constant_node_editor() final = default;

private:
	widget_factory const& _widget_factory;
	clk::constant_node* _constant_node;
	std::unordered_map<clk::output*, std::unique_ptr<clk::gui::editor>> _constant_editors;

	void draw_outputs() final;
};

inline node_editor::node_editor(clk::node* node, int id, widget_cache<clk::port, port_editor>* port_cache,
	std::optional<std::function<bool()>>& queued_action, bool const& draw_node_titles)
	: _queued_action(queued_action), _port_cache(port_cache), _node(node), _id(id), _draw_node_titles(draw_node_titles)
{
}

inline auto node_editor::id() const -> int
{
	return _id;
}

inline auto node_editor::node() const -> clk::node*
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
		ImNodes::PushColorStyle(ImNodesCol_NodeOutline, color_rgba{1.0f}.packed());

	ImNodes::BeginNode(_id);

	ImNodes::BeginNodeTitleBar();
	ImGui::BeginGroup();
	if(!_first_draw && _title_width < _contents_width)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (_contents_width - _title_width) / 2);
	draw_title_bar();
	ImGui::EndGroup();
	if(_first_draw)
		_title_width = ImGui::GetItemRectSize().x;
	ImNodes::EndNodeTitleBar();

	ImGui::BeginGroup();
	draw_inputs();
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	draw_outputs();
	ImGui::EndGroup();

	ImNodes::EndNode();
	_contents_width = ImGui::GetItemRectSize().x;

	if(_highlighted)
		ImNodes::PopColorStyle();

	_first_draw = false;
}

inline void node_editor::draw_title_bar()
{
	if(!_node->inputs().empty())
	{
		if(ImGui::SmallButton("Pull"))
			_node->pull();
		ImGui::SameLine();
	}

	if(_draw_node_titles)
	{
		ImGui::Text("%s", _node->name().data());
	}

	if(!_node->outputs().empty())
	{
		ImGui::SameLine();
		if(ImGui::SmallButton("Push"))
			_node->push();
	}
}

inline void node_editor::draw_inputs()
{
	for(auto* port : _node->inputs())
		_port_cache->widget_for(port).draw();
}

inline void node_editor::draw_outputs()
{
	for(auto* port : _node->outputs())
		_port_cache->widget_for(port).draw();
}

inline constant_node_editor::constant_node_editor(clk::constant_node* constant_node, int id,
	widget_cache<clk::port, port_editor>* port_cache, std::optional<std::function<bool()>>& queued_action,
	widget_factory const& widget_factory, bool const& draw_node_titles)
	: node_editor(constant_node, id, port_cache, queued_action, draw_node_titles)
	, _widget_factory(widget_factory)
	, _constant_node(constant_node)
{
}

inline void constant_node_editor::draw_outputs()
{
	for(auto* port : _constant_node->outputs())
	{
		ImGui::PushID(port);
		if(ImGui::SmallButton("-"))
		{
			if(!_queued_action.has_value())
			{
				_queued_action = [&]() {
					_constant_editors.erase(port);
					_constant_node->remove_output(port);
					return true;
				};
			}
		}
		ImGui::PopID();
		ImGui::SameLine();
		if(_constant_editors.count(port) == 0)
		{
			_constant_editors[port] =
				_widget_factory.create(clk::gui::data_writer<void>{[=]() {
																	   return port->data_pointer();
																   },
										   [=]() {
											   port->update_timestamp();
											   port->push();
										   }},
					port->data_type_hash(), port->name());
			_constant_editors[port]->set_maximum_width(200);
		}

		_port_cache->widget_for(port).draw(_constant_editors[port].get());
	}

	if(ImGui::SmallButton("+"))
	{
		if(!_queued_action.has_value())
		{
			_queued_action = [&]() {
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
						_constant_node->add_output(std::move(constantPort));
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
	std::optional<std::function<bool()>>& queued_action, widget_factory const& widget_factory,
	bool const& draw_node_titles) -> std::unique_ptr<node_editor>
{
	if(auto* constant_node = dynamic_cast<clk::constant_node*>(node))
		return std::make_unique<constant_node_editor>(
			constant_node, id, portCache, queued_action, widget_factory, draw_node_titles);
	else
		return std::make_unique<node_editor>(node, id, portCache, queued_action, draw_node_titles);
}

} // namespace clk::gui::impl