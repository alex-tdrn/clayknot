#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

namespace clk::gui::impl
{
template <typename data_type, typename widget>
class widget_cache
{
public:
	using factory = std::function<std::unique_ptr<widget>(data_type*, int)>;

	widget_cache() = delete;
	widget_cache(factory make_widget);
	widget_cache(widget_cache const&) = delete;
	widget_cache(widget_cache&&) = delete;
	auto operator=(widget_cache const&) -> widget_cache& = delete;
	auto operator=(widget_cache&&) -> widget_cache& = delete;
	~widget_cache() = default;

	auto has_widget(data_type* data) const -> bool;
	auto has_widget(int id) const -> bool;
	auto get_widget(data_type* data) -> widget&;
	auto get_widget(int id) -> widget&;
	auto get_map() const -> std::unordered_map<data_type*, std::unique_ptr<widget>> const&;

private:
	factory _make_widget;
	std::unordered_map<data_type*, std::unique_ptr<widget>> _data_type_to_widget;
	std::unordered_map<int, widget*> _id_to_widget;
	int _next_available_id = 0;
};

template <typename data_type, typename Widget>
inline widget_cache<data_type, Widget>::widget_cache(factory make_widget) : _make_widget(make_widget)
{
}

template <typename data_type, typename widget>
inline auto widget_cache<data_type, widget>::has_widget(data_type* data) const -> bool
{
	return _data_type_to_widget.find(data) != _data_type_to_widget.end();
}

template <typename data_type, typename widget>
inline auto widget_cache<data_type, widget>::has_widget(int id) const -> bool
{
	return _id_to_widget.find(id) != _id_to_widget.end();
}

template <typename data_type, typename widget>
inline auto widget_cache<data_type, widget>::get_widget(data_type* data) -> widget&
{
	if(!has_widget(data))
	{
		_data_type_to_widget.insert({data, _make_widget(data, _next_available_id)});
		_id_to_widget[_next_available_id] = _data_type_to_widget[data].get();
		_next_available_id++;
	}
	return *_data_type_to_widget[data];
}

template <typename data_type, typename widget>
inline auto widget_cache<data_type, widget>::get_widget(int id) -> widget&
{
	if(!has_widget(id))
		throw std::exception("No widget with this id found");

	return *_id_to_widget[id];
}

template <typename data_type, typename widget>
inline auto widget_cache<data_type, widget>::get_map() const
	-> std::unordered_map<data_type*, std::unique_ptr<widget>> const&
{
	return _data_type_to_widget;
}

} // namespace clk::gui::impl
