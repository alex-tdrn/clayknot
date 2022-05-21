#pragma once

#include "clk/base/port.hpp"

#include <memory>
#include <string>
#include <string_view>

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
	auto error() const -> std::string const&;

protected:
	void clear_error() const;
	void set_error(std::string_view error_message) const;

private:
	mutable std::string _last_error_message;
};

} // namespace clk