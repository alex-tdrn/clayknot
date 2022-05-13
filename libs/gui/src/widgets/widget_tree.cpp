#include "clk/gui/widgets/widget_tree.hpp"

#include <imgui.h>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/functional/identity.hpp>
#include <utility>

namespace clk::gui
{

auto widget_tree::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<widget_tree>(this->name());
	clone->copy(*this);
	return clone;
}

void widget_tree::copy(widget const& other)
{
	auto const& casted = dynamic_cast<widget_tree const&>(other);
	_widgets.reserve(casted._widgets.size());
	for(auto const& widget : casted._widgets)
	{
		_widgets.push_back(widget->clone());
	}

	_subtrees.reserve(casted._subtrees.size());
	for(auto const& subtree : casted._subtrees)
	{
		auto cloned_subtree = std::unique_ptr<widget_tree>(static_cast<widget_tree*>(subtree->clone().release()));
		_subtrees.push_back(std::move(cloned_subtree));
	}
	_draw_mode = casted._draw_mode;
	_draw_inline = casted._draw_inline;

	widget::copy(other);
}

auto widget_tree::get_subtree(std::string_view path) -> widget_tree&
{
	if(path.empty())
	{
		return *this;
	}
	auto make_subtree = [&](std::string_view name) -> widget_tree& {
		auto subtree = std::make_unique<widget_tree>(name);
		subtree->_draw_inline = false;
		subtree->disable_title();
		subtree->_draw_mode = _draw_mode;
		_subtrees.push_back(std::move(subtree));
		return *_subtrees.back().get();
	};

	if(auto first_delimiter = path.find_first_of('/'); first_delimiter != std::string_view::npos)
	{
		auto subtree_name = path.substr(0, first_delimiter);
		auto subpath = path.substr(first_delimiter + 1, path.size() - first_delimiter);

		if(auto found_it = ranges::find_if(_subtrees,
			   [&](const auto& subtree) {
				   return subtree->name() == subtree_name;
			   });
			found_it != _subtrees.end())
		{
			return get_subtree(subpath);
		}
		else
		{
			return make_subtree(subtree_name).get_subtree(subpath);
		}
	}
	else
	{
		return make_subtree(path);
	}
}

void widget_tree::add(std::unique_ptr<widget> widget)
{
	_widgets.push_back(std::move(widget));
}

void widget_tree::set_draw_mode(draw_mode mode)
{
	_draw_mode = mode;
	for(auto& subtree : _subtrees)
		subtree->set_draw_mode(mode);
}

void widget_tree::draw_contents() const
{
	auto draw_internal = [&]() {
		for(auto const& widget : _widgets)
			widget->draw();
		for(auto const& subtree : _subtrees)
			subtree->draw();
	};

	if(_draw_inline)
	{
		draw_internal();
	}
	else
	{
		switch(_draw_mode)
		{
			case draw_mode::tree_nodes:
				if(ImGui::TreeNodeEx(name().data(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					draw_internal();
					ImGui::TreePop();
				}
				break;
			case draw_mode::menu:
				if(ImGui::BeginMenu(name().data()))
				{
					draw_internal();
					ImGui::EndMenu();
				}
				break;
		};
	}
}
} // namespace clk::gui