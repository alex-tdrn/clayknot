#pragma once

#include "clk/gui/widgets/widget.hpp"

#include <memory>
#include <string_view>

namespace clk::gui
{
class widget_factory;

class data_widget : public widget
{
public:
    data_widget() = delete;
    data_widget(std::shared_ptr<widget_factory const> factory, std::string_view name);
    data_widget(data_widget const&) = delete;
    data_widget(data_widget&&) = delete;
    auto operator=(data_widget const&) -> data_widget& = delete;
    auto operator=(data_widget&&) -> data_widget& = delete;
    ~data_widget() override;

    auto get_widget_factory() const -> std::shared_ptr<widget_factory const> const&;
    void set_widget_factory(std::shared_ptr<widget_factory const> factory);
    virtual auto data_type_hash() const -> std::size_t = 0;

private:
    std::shared_ptr<widget_factory const> _factory;
};

} // namespace clk::gui
