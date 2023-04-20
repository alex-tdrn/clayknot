#pragma once

#include "clk/base/node.hpp"
#include "clk/util/timestamp.hpp"

#include <memory>
#include <vector>

namespace clk
{
class graph final
{
public:
    graph() = default;
    graph(graph const&) = delete;
    graph(graph&&) = default;
    auto operator=(graph const&) -> graph& = delete;
    auto operator=(graph&&) -> graph& = default;
    ~graph() = default;

    void add_node(std::unique_ptr<clk::node>&& node);
    void remove_node(clk::node* node);
    auto nodes() const -> std::vector<std::unique_ptr<clk::node>> const&;
    auto timestamp() const -> clk::timestamp;

private:
    clk::timestamp _timestamp;
    std::vector<std::unique_ptr<clk::node>> _nodes;
};

} // namespace clk
