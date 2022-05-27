#include "clk/base/split_node.hpp"

namespace clk
{
split_node::split_node()
{
	register_port(&_in);
	register_port(&_out);
}

auto split_node::name() const -> std::string_view
{
	return "Split";
}

void split_node::update()
{
	_out.set_data(_in.data_pointer(), _in.data_type_hash());
}

} // namespace clk