#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/base/node.hpp"
#include "clk/base/port.hpp"

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
	auto inputs() const -> port_range<clk::input*> final;
	auto outputs() const -> port_range<clk::output*> final;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) final;
	void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) final;

private:
	std::unique_ptr<clk::algorithm> _algorithm;
	std::weak_ptr<clk::sentinel> _sentinel;

	auto sentinel_present() const -> bool;
	auto update_possible() const -> bool;
	auto update_needed() const -> bool;
	void try_update() const;
};

} // namespace clk