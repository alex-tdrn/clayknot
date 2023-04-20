#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/base/node.hpp"

#include <memory>
#include <string_view>

namespace clk
{
class input;
class output;
class sentinel;

class algorithm_node final : public node
{
public:
    algorithm_node() = default;
    explicit algorithm_node(std::unique_ptr<clk::algorithm>&& algorithm);
    algorithm_node(algorithm_node const&) = delete;
    algorithm_node(algorithm_node&&) noexcept = delete;
    auto operator=(algorithm_node const&) -> algorithm_node& = delete;
    auto operator=(algorithm_node&&) noexcept -> algorithm_node& = delete;
    ~algorithm_node() final = default;

    auto name() const -> std::string_view final;
    void set_algorithm(std::unique_ptr<clk::algorithm>&& algorithm);

private:
    std::unique_ptr<clk::algorithm> _algorithm;

    auto update_possible() const -> bool override;
    void update() override;
};

} // namespace clk
