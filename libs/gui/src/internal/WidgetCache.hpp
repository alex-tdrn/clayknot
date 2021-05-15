#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

namespace rsp::gui::impl
{
template <typename Data, typename Widget>
class WidgetCache
{
public:
	using Factory = std::function<std::unique_ptr<Widget>(Data*, int)>;

	WidgetCache() = delete;
	WidgetCache(Factory factory);
	WidgetCache(WidgetCache const&) = delete;
	WidgetCache(WidgetCache&&) = delete;
	auto operator=(WidgetCache const&) -> WidgetCache& = delete;
	auto operator=(WidgetCache&&) -> WidgetCache& = delete;
	~WidgetCache() = default;

	auto hasWidget(Data* data) const -> bool;
	auto hasWidget(int id) const -> bool;
	auto getWidget(Data* data) -> Widget&;
	auto getWidget(int id) -> Widget&;
	auto getMap() const -> std::unordered_map<Data*, std::unique_ptr<Widget>> const&;

private:
	Factory factory;
	std::unordered_map<Data*, std::unique_ptr<Widget>> dataTypeToWidget;
	std::unordered_map<int, Widget*> idToWidget;
	int nextAvailableID = 0;
};

template <typename Data, typename Widget>
inline WidgetCache<Data, Widget>::WidgetCache(Factory factory) : factory(factory)
{
}

template <typename Data, typename Widget>
inline auto WidgetCache<Data, Widget>::hasWidget(Data* data) const -> bool
{
	return dataTypeToWidget.find(data) != dataTypeToWidget.end();
}

template <typename Data, typename Widget>
inline auto WidgetCache<Data, Widget>::hasWidget(int id) const -> bool
{
	return idToWidget.find(id) != idToWidget.end();
}

template <typename Data, typename Widget>
inline auto WidgetCache<Data, Widget>::getWidget(Data* data) -> Widget&
{
	if(!hasWidget(data))
	{
		dataTypeToWidget.insert({data, factory(data, nextAvailableID)});
		idToWidget[nextAvailableID] = dataTypeToWidget[data].get();
		nextAvailableID++;
	}
	return *dataTypeToWidget[data];
}

template <typename Data, typename Widget>
inline auto WidgetCache<Data, Widget>::getWidget(int id) -> Widget&
{
	if(!hasWidget(id))
		throw std::exception("No widget with this id found");

	return *idToWidget[id];
}

template <typename Data, typename Widget>
inline auto WidgetCache<Data, Widget>::getMap() const -> std::unordered_map<Data*, std::unique_ptr<Widget>> const&
{
	return dataTypeToWidget;
}

} // namespace rsp::gui::impl
