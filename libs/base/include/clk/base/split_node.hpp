#pragma once

#include "clk/base/any_input.hpp"
#include "clk/base/any_output.hpp"
#include "clk/base/node.hpp"

namespace clk
{

class split_node : public node
{
public:
	split_node();
	split_node(split_node const&) = delete;
	split_node(split_node&&) noexcept = delete;
	auto operator=(split_node const&) -> split_node& = delete;
	auto operator=(split_node&&) noexcept -> split_node& = delete;
	~split_node() override = default;

	auto name() const -> std::string_view override;

private:
	any_input _in{"In"};
	any_output _out{"Out"};

	void update() override;
};
} // namespace clk