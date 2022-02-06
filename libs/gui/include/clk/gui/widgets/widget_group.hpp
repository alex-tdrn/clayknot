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

class widget_group : public widget
{
public:
	enum class draw_mode
	{
		tree,
		menu
	};

	using widget::widget;
	widget_group() = delete;
	widget_group(widget_group const&) = delete;
	widget_group(widget_group&&) = delete;
	auto operator=(widget_group const&) -> widget_group& = delete;
	auto operator=(widget_group&&) -> widget_group& = delete;
	~widget_group() override = default;

	auto clone() const -> std::unique_ptr<widget> override;
	void copy(widget const& other) override;

	template <typename data_type>
	void add(data_type& data, std::string_view name);
	void add(std::unique_ptr<widget> widget);

	void set_draw_mode(draw_mode mode);

	void draw_contents() const override;

private:
	class group
	{
	public:
		group() = default;
		group(group const& other);
		group(group&&) = default;
		auto operator=(group const& other) -> group&;
		auto operator=(group&&) -> group& = default;
		~group() = default;

		void set_name(std::string_view name);
		void add(std::unique_ptr<widget> widget);
		void draw(draw_mode mode, bool is_root) const;

	private:
		std::string _name;
		std::vector<std::unique_ptr<widget>> _widgets;
		std::vector<group> _sub_groups;
	};

	group _root;
	draw_mode _draw_mode = draw_mode::tree;
};

inline auto widget_group::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<widget_group>(this->get_widget_factory(), this->name());
	clone->copy(*this);
	return clone;
}

inline void widget_group::copy(widget const& other)
{
	auto const& casted = dynamic_cast<widget_group const&>(other);
	_root = casted._root;
	widget::copy(other);
}

template <typename data_type>
void widget_group::add(data_type& data, std::string_view name)
{
	add(get_widget_factory()->create(data, name));
}

inline void widget_group::add(std::unique_ptr<widget> widget)
{
	_root.add(std::move(widget));
}

inline void widget_group::set_draw_mode(draw_mode mode)
{
	_draw_mode = mode;
}

inline void widget_group::draw_contents() const
{
	_root.draw(_draw_mode, true);
}

inline widget_group::group::group(group const& other)
{
	*this = other;
}

inline auto widget_group::group::operator=(group const& other) -> group&
{
	if(&other != this)
	{
		_name = other._name;
		_widgets.reserve(other._widgets.size());
		for(auto const& widget : other._widgets)
		{
			_widgets.push_back(widget->clone());
		}
		_sub_groups = other._sub_groups;
	}
	return *this;
}

inline void widget_group::group::set_name(std::string_view name)
{
	_name = std::string(name);
}

inline void widget_group::group::add(std::unique_ptr<widget> widget)
{
	auto name = std::string(widget->name());
	if(auto first_delimiter = name.find_first_of('/'); first_delimiter != std::string_view::npos)
	{
		auto sub_group_name = name.substr(0, first_delimiter);
		auto sub_setting_name = name.substr(first_delimiter + 1, name.size() - first_delimiter);
		widget->set_name(sub_setting_name);

		if(auto found_it = ranges::find_if(_sub_groups,
			   [&](const auto& group) {
				   return group._name == sub_group_name;
			   });
			found_it != _sub_groups.end())
		{
			found_it->add(std::move(widget));
		}
		else
		{
			group sub_group;
			sub_group.set_name(sub_group_name);
			sub_group.add(std::move(widget));
			_sub_groups.push_back(std::move(sub_group));
		}
	}
	else
	{
		_widgets.push_back(std::move(widget));
	}
}

inline void widget_group::group::draw(draw_mode mode, bool is_root) const
{
	auto draw_internal = [&]() {
		for(auto const& widget : _widgets)
			widget->draw();
		for(auto const& subgroup : _sub_groups)
			subgroup.draw(mode, false);
	};

	if(is_root)
	{
		draw_internal();
	}
	else
	{
		switch(mode)
		{
			case draw_mode::tree:
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