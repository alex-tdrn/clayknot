#pragma once

#include "clk/base/port.hpp"

#include <memory>
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
	node() = default;
	node(node const&) = delete;
	node(node&&) noexcept = delete;
	auto operator=(node const&) -> node& = delete;
	auto operator=(node&&) noexcept -> node& = delete;
	virtual ~node() = default;

	virtual auto get_name() const -> std::string const& = 0;
	virtual auto get_input_ports() const -> std::vector<clk::input_port*>;
	virtual auto get_output_ports() const -> std::vector<clk::output_port*>;
	virtual void pull(std::weak_ptr<clk::sentinel> const& sentinel = {});
	virtual void push(std::weak_ptr<clk::sentinel> const& sentinel = {});
};

inline auto node::get_input_ports() const -> std::vector<clk::input_port*>
{
	return {};
}

inline auto node::get_output_ports() const -> std::vector<clk::output_port*>
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

} // namespace clk