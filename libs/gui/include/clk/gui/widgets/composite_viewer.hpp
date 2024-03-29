#pragma once

#include "clk/gui/widgets/viewer.hpp"
#include "clk/gui/widgets/widget_factory.hpp"

#include <functional>
#include <utility>
#include <vector>

namespace clk::gui
{

template <typename DataType>
class composite_viewer_of final : public viewer_of<DataType>
{
public:
    using viewer_of<DataType>::viewer_of;
    composite_viewer_of() = delete;
    composite_viewer_of(composite_viewer_of const&) = delete;
    composite_viewer_of(composite_viewer_of&&) = delete;
    auto operator=(composite_viewer_of const&) -> composite_viewer_of& = delete;
    auto operator=(composite_viewer_of&&) -> composite_viewer_of& = delete;
    ~composite_viewer_of() override = default;

    auto clone() const -> std::unique_ptr<widget> override
    {
        auto clone = std::make_unique<composite_viewer_of<DataType>>(this->get_widget_factory(), this->name());
        clone->copy(*this);
        return clone;
    }

    void copy(widget const& other) override
    {
        auto const& casted = dynamic_cast<composite_viewer_of<DataType> const&>(other);
        _viewer_cloners.clear();
        for(auto const& cloner : casted._viewer_cloners)
        {
            _viewer_cloners.push_back(cloner);
        }

        viewer_of<DataType>::copy(other);
    }

    template <typename SubDataType>
    void add_sub_viewer(SubDataType DataType::*data_member, std::string_view name)
    {
        viewer_cloner cloner = [=](widget_factory const& factory, DataType const*& data_proxy) {
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
        _viewers.push_back(cloner(*(this->get_widget_factory()), _data_proxy));
        _viewer_cloners.push_back(std::move(cloner));
    }

    template <typename SubDataType>
    void add_sub_viewer(std::function<SubDataType const*(DataType const&)> sub_data_getter, std::string_view name)
    {
        viewer_cloner cloner = [=, sub_data_getter = std::move(sub_data_getter)](
                                   widget_factory const& factory, DataType const*& data_proxy) {
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
        _viewers.push_back(cloner(*(this->get_widget_factory()), _data_proxy));
        _viewer_cloners.push_back(std::move(cloner));
    }

private:
    using viewer_cloner = std::function<std::unique_ptr<viewer>(widget_factory const&, DataType const*&)>;

    mutable DataType const* _data_proxy = nullptr;
    std::vector<std::unique_ptr<viewer>> _viewers;
    std::vector<viewer_cloner> _viewer_cloners;

    void draw_contents(DataType const& data) const final
    {
        _data_proxy = &data;

        if(_viewers.empty())
        {
            ImGui::Text("No sub-viewer set up for composite viewer");
        }
        else
        {
            for(auto const& viewer : _viewers)
            {
                viewer->draw();
            }
        }

        _data_proxy = nullptr;
    }
};

} // namespace clk::gui
