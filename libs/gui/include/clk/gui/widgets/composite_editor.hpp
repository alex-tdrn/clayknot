#pragma once

#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/widget_factory.hpp"

#include <functional>
#include <utility>
#include <vector>

namespace clk::gui
{

template <typename DataType>
class composite_editor_of final : public editor_of<DataType>
{
public:
    using editor_of<DataType>::editor_of;
    composite_editor_of() = delete;
    composite_editor_of(composite_editor_of const&) = delete;
    composite_editor_of(composite_editor_of&&) = delete;
    auto operator=(composite_editor_of const&) -> composite_editor_of& = delete;
    auto operator=(composite_editor_of&&) -> composite_editor_of& = delete;
    ~composite_editor_of() override = default;

    auto clone() const -> std::unique_ptr<widget> override
    {
        auto clone = std::make_unique<composite_editor_of<DataType>>(this->get_widget_factory(), this->name());
        clone->copy(*this);
        return clone;
    }

    void copy(widget const& other) override
    {
        auto const& casted = dynamic_cast<composite_editor_of<DataType> const&>(other);
        _widget_cloners.clear();
        for(auto const& cloner : casted._widget_cloners)
        {
            _widget_cloners.push_back(cloner);
        }

        editor_of<DataType>::copy(other);
    }

    template <typename SubDataType>
    void add_sub_viewer(SubDataType DataType::*data_member, std::string_view name)
    {
        widget_cloner cloner = [=](widget_factory const& factory, DataType*& data_proxy) {
            return factory.create(data_reader<SubDataType>([&]() -> SubDataType const* {
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

    template <typename SubDataType>
    void add_sub_viewer(std::function<SubDataType const*(DataType const&)> sub_data_getter, std::string_view name)
    {
        widget_cloner cloner = [=, sub_data_getter = std::move(sub_data_getter)](
                                   widget_factory const& factory, DataType*& data_proxy) {
            return factory.create(data_reader<SubDataType>([&]() -> SubDataType const* {
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

    template <typename SubDataType>
    void add_sub_editor(SubDataType DataType::*data_member, std::string_view name)
    {
        widget_cloner cloner = [=](widget_factory const& factory, DataType*& data_proxy) {
            return factory.create(data_writer<SubDataType>([&]() -> SubDataType* {
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

    template <typename SubDataType>
    void add_sub_editor(std::function<SubDataType*(DataType&)> sub_data_getter, std::string_view name)
    {
        widget_cloner cloner = [=, sub_data_getter = std::move(sub_data_getter)](
                                   widget_factory const& factory, DataType*& data_proxy) {
            return factory.create(data_writer<SubDataType>([&]() -> SubDataType* {
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

    template <typename SubDataType>
    void add_sub_editor(std::function<SubDataType*(DataType&)> sub_data_getter,
        std::function<void(DataType&, SubDataType*)> sub_data_setter, std::string_view name)
    {
        widget_cloner cloner = [=, sub_data_getter = std::move(sub_data_getter),
                                   sub_data_setter = std::move(sub_data_setter)](
                                   widget_factory const& factory, DataType*& data_proxy) {
            return factory.create(
                data_reader<SubDataType>([&]() -> SubDataType* {
                    if(data_proxy != nullptr)
                    {
                        return sub_data_getter(*data_proxy);
                    }
                    else
                    {
                        return nullptr;
                    }
                }),
                [&](SubDataType* new_sub_data) {
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

private:
    using widget_cloner = std::function<std::unique_ptr<widget>(widget_factory const&, DataType*&)>;

    mutable DataType* _data_proxy = nullptr;
    std::vector<std::unique_ptr<widget>> _widgets;
    std::vector<widget_cloner> _widget_cloners;

    auto draw_contents(DataType& data) const -> bool final
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
};

} // namespace clk::gui
