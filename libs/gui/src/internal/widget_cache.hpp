#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace clk::gui::impl
{
template <typename DataType, typename Widget>
class widget_cache
{
public:
    using factory = std::function<std::unique_ptr<Widget>(DataType*, int)>;

    widget_cache() = delete;

    explicit widget_cache(factory make_widget) : _make_widget(std::move(make_widget))
    {
    }

    widget_cache(widget_cache const&) = delete;
    widget_cache(widget_cache&&) = delete;
    auto operator=(widget_cache const&) -> widget_cache& = delete;
    auto operator=(widget_cache&&) -> widget_cache& = delete;
    ~widget_cache() = default;

    auto has_widget_for(DataType* data) const -> bool
    {
        return _data_type_to_widget.find(data) != _data_type_to_widget.end();
    }

    auto has_widget_for(int id) const -> bool
    {
        return _id_to_widget.find(id) != _id_to_widget.end();
    }

    auto widget_for(DataType* data) -> Widget&
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

    auto widget_for(int id) -> Widget&
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

private:
    factory _make_widget;
    std::unordered_map<DataType*, std::unique_ptr<Widget>> _data_type_to_widget;
    std::unordered_map<int, Widget*> _id_to_widget;
    int _next_available_id = 0;
};

} // namespace clk::gui::impl
