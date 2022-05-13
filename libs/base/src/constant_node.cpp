#include "clk/base/constant_node.hpp"
#include "clk/util/predicates.hpp"
#include "clk/util/projections.hpp"

#include <range/v3/algorithm/remove_if.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/any_view.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

namespace clk
{

auto constant_node::name() const -> std::string_view
{
	return "Constant";
}

auto constant_node::outputs() const -> port_range<clk::output*>
{
	return _outputs | ranges::views::transform(clk::projections::underlying());
}

void constant_node::remove_output(clk::output* output)
{
	_outputs.erase(ranges::remove_if(_outputs, clk::predicates::is_equal_to(output), clk::projections::underlying()),
		_outputs.end());
}

void constant_node::add_output(std::unique_ptr<clk::output>&& output)
{
	_outputs.push_back(std::move(output));
}
} // namespace clk