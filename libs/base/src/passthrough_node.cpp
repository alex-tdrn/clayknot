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
    _out.set_data(_in.data_pointer(), _in.data_type_hash());
}

} // namespace clk
