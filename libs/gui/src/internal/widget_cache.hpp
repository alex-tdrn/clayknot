#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace clk::gui::impl
{
template <typename data_type, typename widget>
class widget_cache
{
public:
	using factory = std::function<std::unique_ptr<widget>(data_type*, int)>;

	widget_cache() = delete;
	explicit widget_cache(factory make_widget);
	widget_cache(widget_cache const&) = delete;
	widget_cache(widget_cache&&) = delete;
	auto operator=(widget_cache const&) -> widget_cache& = delete;
	auto operator=(widget_cache&&) -> widget_cache& = delete;
	~widget_cache() = default;

	auto has_widget_for(data_type* data) const -> bool;
	auto has_widget_for(int id) const -> bool;
	auto widget_for(data_type* data) -> widget&;
	auto widget_for(int id) -> widget&;
	auto map() const -> std::unordered_map<data_type*, std::unique_ptr<widget>> const&;

private:
	factory _make_widget;
	std::unordered_map<data_type*, std::unique_ptr<widget>> _data_type_to_widget;
	std::unordered_map<int, widget*> _id_to_widget;
	int _next_available_id = 0;
};

template <typename data_type, typename Widget>
inline widget_cache<data_type, Widget>::widget_cache(factory make_widget) : _make_widget(std::move(make_widget))
{
}

template <typename data_type, typename widget>
inline auto widget_cache<data_type, widget>::has_widget_for(data_type* data) const -> bool
{
	return _data_type_to_widget.find(data) != _data_type_to_widget.end();
}

template <typename data_type, typename widget>
inline auto widget_cache<data_type, widget>::has_widget_for(int id) const -> bool
{
	return _id_to_widget.find(id) != _id_to_widget.end();
}

template <typename data_type, typename widget>
inline auto widget_cache<data_type, widget>::widget_for(data_type* data) -> widget&
{
	if(auto found_it = _data_type_to_widget.find(data); found_it != _data_type_to_widget.end())
	{
		return *(found_it->second.get());
	}
	else
	{
		auto new_widget = _make_widget(data, _next_available_id);
		auto& widget_ref = *new_widget;
		_id_to_widget.insert({_next_available_id, new_widget.get()});
		_data_type_to_widget.insert({data, std::move(new_widget)});
		_next_available_id++;
		return widget_ref;
	}
}

template <typename data_type, typename widget>
inline auto widget_cache<data_type, widget>::widget_for(int id) -> widget&
{
	if(auto found_it = _id_to_widget.find(id); found_it != _id_to_widget.end())
	{
		return *(found_it->second);
	}
	else
	{
		throw std::runtime_error("No widget with this id found");
	}
}

} // namespace clk::gui::impl
