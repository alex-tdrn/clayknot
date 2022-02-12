#include "clk/gui/widgets/data_widget.hpp"

#include "clk/gui/widgets/widget_factory.hpp"

namespace clk::gui
{
data_widget::data_widget(std::shared_ptr<widget_factory const> factory, std::string_view name)
	: widget(name), _factory(std::move(factory))
{
}

data_widget::~data_widget() = default;

auto data_widget::get_widget_factory() const -> std::shared_ptr<widget_factory const> const&
{
	return _factory;
}

void data_widget::set_widget_factory(std::shared_ptr<widget_factory const> factory)
{
	_factory = std::move(factory);
}

} // namespace clk::gui