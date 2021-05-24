#pragma once

#include "clk/base/port.hpp"

#include <memory>
#include <range/v3/view.hpp>
#include <string>
#include <vector>

namespace clk
{
class sentinel;
class input_port;
class output_port;

class node
{
public:
	template <typename T>
	using port_range = ranges::any_view<T, ranges::category::bidirectional | ranges::category::sized>;

	node() = default;
	node(node const&) = delete;
	node(node&&) noexcept = delete;
	auto operator=(node const&) -> node& = delete;
	auto operator=(node&&) noexcept -> node& = delete;
	virtual ~node() = default;

	virtual auto get_name() const -> std::string const& = 0;
	auto get_ports() const -> port_range<clk::port*>;
	virtual auto get_input_ports() const -> port_range<clk::input_port*>;
	virtual auto get_output_ports() const -> port_range<clk::output_port*>;
	virtual void pull(std::weak_ptr<clk::sentinel> const& sentinel = {});
	virtual void push(std::weak_ptr<clk::sentinel> const& sentinel = {});

	auto is_source() const -> bool;
	auto is_sink() const -> bool;
};

inline auto node::get_ports() const -> port_range<clk::port*>
{
	return ranges::views::concat(get_input_ports() | ranges::views::transform(clk::projections::cast<clk::port*>()),
		get_output_ports() | ranges::views::transform(clk::projections::cast<clk::port*>()));
}

inline auto node::get_input_ports() const -> port_range<clk::input_port*>
{
	return {};
}

inline auto node::get_output_ports() const -> port_range<clk::output_port*>
{
	return {};
}

inline void node::pull(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* port : get_input_ports())
		port->pull(sentinel);
}

inline void node::push(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* port : get_output_ports())
		port->push(sentinel);
}

inline auto node::is_source() const -> bool
{
	return get_input_ports().size() == 0;
}

inline auto node::is_sink() const -> bool
{
	return get_output_ports().size() == 0;
}

} // namespace clk