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

	virtual auto get_name() const -> std::string const& = 0;
	auto get_ports() const -> port_range<clk::port*>;
	virtual auto get_inputs() const -> port_range<clk::input*>;
	virtual auto get_outputs() const -> port_range<clk::output*>;
	virtual void pull(std::weak_ptr<clk::sentinel> const& sentinel = {});
	virtual void push(std::weak_ptr<clk::sentinel> const& sentinel = {});

	auto is_source() const -> bool;
	auto is_sink() const -> bool;
};

inline auto node::get_ports() const -> port_range<clk::port*>
{
	return ranges::views::concat(get_inputs() | ranges::views::transform(clk::projections::cast<clk::port*>()),
		get_outputs() | ranges::views::transform(clk::projections::cast<clk::port*>()));
}

inline auto node::get_inputs() const -> port_range<clk::input*>
{
	return {};
}

inline auto node::get_outputs() const -> port_range<clk::output*>
{
	return {};
}

inline void node::pull(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* port : get_inputs())
		port->pull(sentinel);
}

inline void node::push(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* port : get_outputs())
		port->push(sentinel);
}

inline auto node::is_source() const -> bool
{
	return get_inputs().size() == 0;
}

inline auto node::is_sink() const -> bool
{
	return get_outputs().size() == 0;
}

} // namespace clk