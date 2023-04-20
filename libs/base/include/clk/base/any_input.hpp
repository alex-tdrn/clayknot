#pragma once

#include "clk/base/input.hpp"

#include "clk/base/any_output.hpp"

namespace clk
{
class output;

class any_input final : public input
{
public:
    any_input();
    explicit any_input(std::string_view name);

    any_input(any_input const&) = delete;
    any_input(any_input&&) = delete;
    auto operator=(any_input const&) -> any_input& = delete;
    auto operator=(any_input&&) -> any_input& = delete;
    ~any_input() final;

    auto data_type_hash() const noexcept -> std::size_t final;
    auto data_pointer() const noexcept -> void const* final;

    auto can_connect_to(port const& other_port) const noexcept -> bool final;

    auto default_port() const -> output& final;
    auto create_compatible_port() const -> std::unique_ptr<port> final;

private:
    any_output mutable _default_port{"Default port"};
};

} // namespace clk
