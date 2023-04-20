#pragma once

#include "clk/base/node.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace clk
{
class output;

class constant_node final : public node
{
public:
    constant_node() = default;
    constant_node(constant_node const&) = delete;
    constant_node(constant_node&&) noexcept = delete;
    auto operator=(constant_node const&) -> constant_node& = delete;
    auto operator=(constant_node&&) noexcept -> constant_node& = delete;
    ~constant_node() final;

    auto name() const -> std::string_view final;
    void remove_output(clk::output* output);
    void add_output(std::unique_ptr<clk::output>&& output);

private:
    std::vector<std::unique_ptr<clk::output>> _outputs;
};

} // namespace clk
