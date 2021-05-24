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

	auto name() const -> std::string const& final;
	auto outputs() const -> port_range<clk::output*> final;
	void remove_output(clk::output* output);
	void add_output(std::unique_ptr<clk::output>&& output);

private:
	std::vector<std::unique_ptr<clk::output>> _outputs;
};

inline auto constant_node::name() const -> std::string const&
{
	static std::string const name = "Constant";
	return name;
}

inline auto constant_node::outputs() const -> port_range<clk::output*>
{
	return _outputs | ranges::views::transform(clk::projections::underlying());
}

inline void constant_node::remove_output(clk::output* output)
{
	_outputs.erase(ranges::remove_if(_outputs, clk::predicates::is_equal_to(output), clk::projections::underlying()),
		_outputs.end());
}

inline void constant_node::add_output(std::unique_ptr<clk::output>&& output)
{
	_outputs.push_back(std::move(output));
}

} // namespace clk