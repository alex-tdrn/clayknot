#include "clk/base/any_input.hpp"
#include "clk/base/any_output.hpp"

#include <memory>
#include <utility>

namespace clk
{
any_input::any_input()
{
    _default_port.connect_to(*this, false);
}

any_input::any_input(std::string_view name) : input(name)
{
    _default_port.connect_to(*this, false);
}

any_input::~any_input()
{
    disconnect(false);
}

auto any_input::data_type_hash() const noexcept -> std::size_t
{
    if(connected_output() != nullptr)
    {
        return connected_output()->data_type_hash();
    }
    return 0;
}

auto any_input::data_pointer() const noexcept -> void const*
{
    if(connected_output() != nullptr)
    {
        return std::as_const(*connected_output()).data_pointer();
    }
    return nullptr;
}

auto any_input::can_connect_to(port const& other_port) const noexcept -> bool
{
    return dynamic_cast<output const*>(&other_port) != nullptr;
}

auto any_input::default_port() const -> output&
{
    return _default_port;
}

auto any_input::create_compatible_port() const -> std::unique_ptr<port>
{
    return std::make_unique<any_output>();
}

} // namespace clk
