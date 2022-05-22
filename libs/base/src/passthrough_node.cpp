#include "clk/base/passthrough_node.hpp"

namespace clk
{
passthrough_node::passthrough_node()
{
	register_port(&_in);
	register_port(&_out);
}

auto passthrough_node::name() const -> std::string_view
{
	return "Passthrough";
}

void passthrough_node::update()
{
}

} // namespace clk