#pragma once

#include "clk/base/node.hpp"
#include "clk/base/port.hpp"
#include "clk/util/predicates.hpp"
#include "clk/util/projections.hpp"

#include <memory>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>
#include <string>
#include <vector>

namespace clk
{
class constant_node final : public node
{
public:
	constant_node() = default;
	constant_node(constant_node const&) = delete;
	constant_node(constant_node&&) noexcept = delete;
	auto operator=(constant_node const&) -> constant_node& = delete;
	auto operator=(constant_node&&) noexcept -> constant_node& = delete;
	~constant_node() final = default;

	auto get_name() const -> std::string const& final;
	auto get_output_ports() const -> std::vector<clk::output_port*> final;
	void remove_port(clk::output_port* port);
	void add_port(std::unique_ptr<clk::output_port>&& port);

private:
	std::vector<std::unique_ptr<clk::output_port>> _outputs;
};

inline auto constant_node::get_name() const -> std::string const&
{
	static std::string const name = "Constant";
	return name;
}

inline auto constant_node::get_output_ports() const -> std::vector<clk::output_port*>
{
	return _outputs | ranges::views::transform(clk::underlying()) | ranges::to<std::vector>();
}

inline void constant_node::remove_port(clk::output_port* port)
{
	_outputs.erase(ranges::remove_if(_outputs, clk::is_equal_to(port), clk::underlying()), _outputs.end());
}

inline void constant_node::add_port(std::unique_ptr<clk::output_port>&& port)
{
	_outputs.push_back(std::move(port));
}

} // namespace clk