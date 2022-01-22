#pragma once

#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/widget_factory.hpp"

#include <functional>
#include <utility>
#include <vector>

namespace clk::gui
{

template <typename data_type>
class composite_editor_of final : public editor_of<data_type>
{
public:
	composite_editor_of(std::shared_ptr<widget_factory> factory, std::string_view name);
	composite_editor_of() = delete;
	composite_editor_of(composite_editor_of const&) = delete;
	composite_editor_of(composite_editor_of&&) = delete;
	auto operator=(composite_editor_of const&) -> composite_editor_of& = delete;
	auto operator=(composite_editor_of&&) -> composite_editor_of& = delete;
	~composite_editor_of() override = default;

	auto clone() const -> std::unique_ptr<widget> override;
	void copy(widget const& other) override;

	template <typename sub_data_type>
	void add_sub_viewer(sub_data_type data_type::*data_member, std::string_view name);

	template <typename sub_data_type>
	void add_sub_viewer(std::function<sub_data_type const*(data_type const&)> sub_data_getter, std::string_view name);

	template <typename sub_data_type>
	void add_sub_editor(sub_data_type data_type::*data_member, std::string_view name);

	template <typename sub_data_type>
	void add_sub_editor(std::function<sub_data_type*(data_type&)> sub_data_getter, std::string_view name);

	template <typename sub_data_type>
	void add_sub_editor(std::function<sub_data_type*(data_type&)> sub_data_getter,
		std::function<void(data_type&, sub_data_type*)> sub_data_setter, std::string_view name);

private:
	using widget_cloner = std::function<std::unique_ptr<widget>(widget_factory const&, data_type*&)>;

	mutable data_type* _data_proxy = nullptr;
	std::vector<std::unique_ptr<widget>> _widgets;
	std::vector<widget_cloner> _widget_cloners;

	auto draw_contents(data_type& data) const -> bool final;
};

template <typename data_type>
composite_editor_of<data_type>::composite_editor_of(std::shared_ptr<widget_factory> factory, std::string_view name)
	: editor_of<data_type>(std::move(factory), name)
{
}

template <typename data_type>
auto composite_editor_of<data_type>::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<composite_editor_of<data_type>>(this->get_widget_factory(), this->name());
	clone->copy(*this);
	return clone;
}

template <typename data_type>
void composite_editor_of<data_type>::copy(widget const& other)
{
	auto const& casted = dynamic_cast<composite_editor_of<data_type> const&>(other);
	_widget_cloners.clear();
	for(auto const& cloner : casted._widget_cloners)
	{
		_widget_cloners.push_back(cloner);
	}

	editor_of<data_type>::copy(other);
}

template <typename data_type>
template <typename sub_data_type>
void composite_editor_of<data_type>::add_sub_viewer(sub_data_type data_type::*data_member, std::string_view name)
{
	widget_cloner cloner = [=](widget_factory const& factory, data_type*& data_proxy) {
		return factory.create(data_reader<sub_data_type>([&]() -> sub_data_type const* {
			if(data_proxy != nullptr)
			{
				return &(data_proxy->*data_member);
			}
			else
			{
				return nullptr;
			}
		}),
			name);
	};
	_widgets.push_back(cloner(*(this->get_widget_factory()), _data_proxy));
	_widget_cloners.push_back(std::move(cloner));
}

template <typename data_type>
template <typename sub_data_type>
void composite_editor_of<data_type>::add_sub_viewer(
	std::function<sub_data_type const*(data_type const&)> sub_data_getter, std::string_view name)
{
	widget_cloner cloner = [=, sub_data_getter = std::move(sub_data_getter)](
							   widget_factory const& factory, data_type*& data_proxy) {
		return factory.create(data_reader<sub_data_type>([&]() -> sub_data_type const* {
			if(data_proxy != nullptr)
			{
				return sub_data_getter(*data_proxy);
			}
			else
			{
				return nullptr;
			}
		}),
			name);
	};
	_widgets.push_back(cloner(*(this->get_widget_factory()), _data_proxy));
	_widget_cloners.push_back(std::move(cloner));
}

template <typename data_type>
template <typename sub_data_type>
void composite_editor_of<data_type>::add_sub_editor(sub_data_type data_type::*data_member, std::string_view name)
{
	widget_cloner cloner = [=](widget_factory const& factory, data_type*& data_proxy) {
		return factory.create(data_writer<sub_data_type>([&]() -> sub_data_type* {
			if(data_proxy != nullptr)
			{
				return &(data_proxy->*data_member);
			}
			else
			{
				return nullptr;
			}
		}),
			name);
	};
	_widgets.push_back(cloner(*(this->get_widget_factory()), _data_proxy));
	_widget_cloners.push_back(std::move(cloner));
}

template <typename data_type>
template <typename sub_data_type>
void composite_editor_of<data_type>::add_sub_editor(
	std::function<sub_data_type*(data_type&)> sub_data_getter, std::string_view name)
{
	widget_cloner cloner = [=, sub_data_getter = std::move(sub_data_getter)](
							   widget_factory const& factory, data_type*& data_proxy) {
		return factory.create(data_writer<sub_data_type>([&]() -> sub_data_type* {
			if(data_proxy != nullptr)
			{
				return sub_data_getter(*data_proxy);
			}
			else
			{
				return nullptr;
			}
		}),
			name);
	};
	_widgets.push_back(cloner(*(this->get_widget_factory()), _data_proxy));
	_widget_cloners.push_back(std::move(cloner));
}

template <typename data_type>
template <typename sub_data_type>
void composite_editor_of<data_type>::add_sub_editor(std::function<sub_data_type*(data_type&)> sub_data_getter,
	std::function<void(data_type&, sub_data_type*)> sub_data_setter, std::string_view name)
{
	widget_cloner cloner = [=, sub_data_getter = std::move(sub_data_getter),
							   sub_data_setter = std::move(sub_data_setter)](
							   widget_factory const& factory, data_type*& data_proxy) {
		return factory.create(
			data_reader<sub_data_type>([&]() -> sub_data_type* {
				if(data_proxy != nullptr)
				{
					return sub_data_getter(*data_proxy);
				}
				else
				{
					return nullptr;
				}
			}),
			[&](sub_data_type* new_sub_data) {
				if(data_proxy != nullptr)
				{
					sub_data_setter(data_proxy, new_sub_data);
				}
			},
			name);
	};
	_widgets.push_back(cloner(*(this->get_widget_factory()), _data_proxy));
	_widget_cloners.push_back(std::move(cloner));
}

template <typename data_type>
auto composite_editor_of<data_type>::draw_contents(data_type& data) const -> bool
{
	_data_proxy = &data;

	if(_widgets.empty())
	{
		ImGui::Text("No sub-editor set up for composite editor");
	}
	else
	{
		for(auto const& editor : _widgets)
		{
			editor->draw();
		}
	}

	_data_proxy = nullptr;

	return false;
}

} // namespace clk::gui