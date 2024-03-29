#include "clk/base/constant_node.hpp"
#include "clk/base/output.hpp"
#include "clk/util/predicates.hpp"
#include "clk/util/projections.hpp"

#include <range/v3/algorithm/remove_if.hpp>
#include <utility>

namespace clk
{

constant_node::~constant_node() = default;

auto constant_node::name() const -> std::string_view
{
    return "Constant";
}

void constant_node::remove_output(clk::output* output)
{
    unregister_port(output);
    _outputs.erase(ranges::remove_if(_outputs, clk::predicates::is_equal_to(output), clk::projections::underlying()),
        _outputs.end());
}

void constant_node::add_output(std::unique_ptr<clk::output>&& output)
{
    register_port(output.get());
    _outputs.push_back(std::move(output));
}
} // namespace clk
