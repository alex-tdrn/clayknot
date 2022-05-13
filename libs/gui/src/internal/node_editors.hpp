#pragma once
#include "clk/gui/widgets/editor.hpp"
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

namespace clk
{
class constant_node;
class node;
class output;
class port;
} // namespace clk

namespace clk::gui
{
class widget_factory;
}

namespace clk::gui::impl
{
class port_editor;
template <typename data_type, typename widget>
class widget_cache;
} // namespace clk::gui::impl

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

auto create_node_editor(clk::node* node, int id, widget_cache<clk::port, port_editor>* portCache,
	std::optional<std::function<bool()>>& queued_action, widget_factory const& widget_factory,
	bool const& draw_node_titles) -> std::unique_ptr<node_editor>;

} // namespace clk::gui::impl