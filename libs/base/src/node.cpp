#include "clk/base/node.hpp"
#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
#include "clk/util/projections.hpp"

#include <range/v3/all.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/utility/get.hpp>
#include <range/v3/view/any_view.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>

namespace clk
{

auto node::all_ports() const -> port_range<clk::port*>
{
	return ranges::views::concat(inputs() | ranges::views::transform(clk::projections::cast<clk::port*>()),
		outputs() | ranges::views::transform(clk::projections::cast<clk::port*>()));
}

auto node::inputs() const -> port_range<clk::input*>
{
	return {};
}

auto node::outputs() const -> port_range<clk::output*>
{
	return {};
}

void node::pull(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* port : inputs())
		port->pull(sentinel);
}

void node::push(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* port : outputs())
		port->push(sentinel);
}

auto node::has_inputs() const -> bool
{
	return inputs().size() != 0;
}

auto node::has_outputs() const -> bool
{
	return outputs().size() != 0;
}

} // namespace clk