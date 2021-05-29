#pragma once

#include "clk/base/input.hpp"
#include "clk/base/output.hpp"

#include <memory>
#include <range/v3/view.hpp>
#include <string>
#include <vector>

namespace clk
{
class sentinel;
class input;
class output;

class node
{
public:
	node() = default;
	node(node const&) = delete;
	node(node&&) noexcept = delete;
	auto operator=(node const&) -> node& = delete;
	auto operator=(node&&) noexcept -> node& = delete;
	virtual ~node() = default;

	virtual auto name() const -> std::string_view = 0;
	auto all_ports() const -> port_range<clk::port*>;
	virtual auto inputs() const -> port_range<clk::input*>;
	virtual auto outputs() const -> port_range<clk::output*>;
	virtual void pull(std::weak_ptr<clk::sentinel> const& sentinel = {});
	virtual void push(std::weak_ptr<clk::sentinel> const& sentinel = {});
	auto has_inputs() const -> bool;
	auto has_outputs() const -> bool;
};

inline auto node::all_ports() const -> port_range<clk::port*>
{
	return ranges::views::concat(inputs() | ranges::views::transform(clk::projections::cast<clk::port*>()),
		outputs() | ranges::views::transform(clk::projections::cast<clk::port*>()));
}

inline auto node::inputs() const -> port_range<clk::input*>
{
	return {};
}

inline auto node::outputs() const -> port_range<clk::output*>
{
	return {};
}

inline void node::pull(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* port : inputs())
		port->pull(sentinel);
}

inline void node::push(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* port : outputs())
		port->push(sentinel);
}

inline auto node::has_inputs() const -> bool
{
	return inputs().size() != 0;
}

inline auto node::has_outputs() const -> bool
{
	return outputs().size() != 0;
}

} // namespace clk