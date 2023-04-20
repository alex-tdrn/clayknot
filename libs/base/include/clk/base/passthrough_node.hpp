#pragma once

#include "clk/base/any_input.hpp"
#include "clk/base/any_output.hpp"
#include "clk/base/node.hpp"

namespace clk
{

class passthrough_node : public node
{
public:
    passthrough_node();
    passthrough_node(passthrough_node const&) = delete;
    passthrough_node(passthrough_node&&) noexcept = delete;
    auto operator=(passthrough_node const&) -> passthrough_node& = delete;
    auto operator=(passthrough_node&&) noexcept -> passthrough_node& = delete;
    ~passthrough_node() override = default;

    auto name() const -> std::string_view override;

private:
    any_input _in{"In"};
    any_output _out{"Out"};

    void update() override;
};
} // namespace clk
