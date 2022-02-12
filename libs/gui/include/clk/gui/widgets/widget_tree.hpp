#pragma once

#include "clk/gui/widgets/widget.hpp"
#include "clk/gui/widgets/widget_factory.hpp"

#include <imgui.h>
#include <memory>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/range.hpp>
#include <string>
#include <vector>

namespace clk::gui
{

class widget_tree : public widget
{
public:
	enum class draw_mode
	{
		tree_nodes,
		menu
	};

	using widget::widget;
	widget_tree() = delete;
	widget_tree(widget_tree const&) = delete;
	widget_tree(widget_tree&&) = delete;
	auto operator=(widget_tree const&) -> widget_tree& = delete;
	auto operator=(widget_tree&&) -> widget_tree& = delete;
	~widget_tree() override = default;

	auto clone() const -> std::unique_ptr<widget> override;
	void copy(widget const& other) override;

	void add(std::unique_ptr<widget> widget);

	void set_draw_mode(draw_mode mode);

	void draw_contents() const override;

private:
	class tree
	{
	public:
		tree() = default;
		tree(tree const& other);
		tree(tree&&) = default;
		auto operator=(tree const& other) -> tree&;
		auto operator=(tree&&) -> tree& = default;
		~tree() = default;

		void set_name(std::string_view name);
		void add(std::unique_ptr<widget> widget);
		void draw(draw_mode mode, bool is_root) const;

	private:
		std::string _name;
		std::vector<std::unique_ptr<widget>> _widgets;
		std::vector<tree> _sub_trees;
	};

	tree _root;
	draw_mode _draw_mode = draw_mode::tree_nodes;
};

inline auto widget_tree::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<widget_tree>(this->name());
	clone->copy(*this);
	return clone;
}

inline void widget_tree::copy(widget const& other)
{
	auto const& casted = dynamic_cast<widget_tree const&>(other);
	_root = casted._root;
	widget::copy(other);
}

inline void widget_tree::add(std::unique_ptr<widget> widget)
{
	_root.add(std::move(widget));
}

inline void widget_tree::set_draw_mode(draw_mode mode)
{
	_draw_mode = mode;
}

inline void widget_tree::draw_contents() const
{
	_root.draw(_draw_mode, true);
}

inline widget_tree::tree::tree(tree const& other)
{
	*this = other;
}

inline auto widget_tree::tree::operator=(tree const& other) -> tree&
{
	if(&other != this)
	{
		_name = other._name;
		_widgets.reserve(other._widgets.size());
		for(auto const& widget : other._widgets)
		{
			_widgets.push_back(widget->clone());
		}
		_sub_trees = other._sub_trees;
	}
	return *this;
}

inline void widget_tree::tree::set_name(std::string_view name)
{
	_name = std::string(name);
}

inline void widget_tree::tree::add(std::unique_ptr<widget> widget)
{
	auto name = std::string(widget->name());
	if(auto first_delimiter = name.find_first_of('/'); first_delimiter != std::string_view::npos)
	{
		auto sub_tree_name = name.substr(0, first_delimiter);
		auto sub_setting_name = name.substr(first_delimiter + 1, name.size() - first_delimiter);
		widget->set_name(sub_setting_name);

		if(auto found_it = ranges::find_if(_sub_trees,
			   [&](const auto& tree) {
				   return tree._name == sub_tree_name;
			   });
			found_it != _sub_trees.end())
		{
			found_it->add(std::move(widget));
		}
		else
		{
			tree sub_tree;
			sub_tree.set_name(sub_tree_name);
			sub_tree.add(std::move(widget));
			_sub_trees.push_back(std::move(sub_tree));
		}
	}
	else
	{
		_widgets.push_back(std::move(widget));
	}
}

inline void widget_tree::tree::draw(draw_mode mode, bool is_root) const
{
	auto draw_internal = [&]() {
		for(auto const& widget : _widgets)
			widget->draw();
		for(auto const& subtree : _sub_trees)
			subtree.draw(mode, false);
	};

	if(is_root)
	{
		draw_internal();
	}
	else
	{
		switch(mode)
		{
			case draw_mode::tree_nodes:
				if(ImGui::TreeNode(_name.c_str()))
				{
					draw_internal();
					ImGui::TreePop();
				}
				break;
			case draw_mode::menu:
				if(ImGui::BeginMenu(_name.c_str()))
				{
					draw_internal();
					ImGui::EndMenu();
				}
				break;
		};
	}
}

} // namespace clk::gui