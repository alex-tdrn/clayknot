#pragma once

#include "clk/base/node.hpp"
#include "clk/base/port.hpp"

#include <algorithm>
#include <memory>
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
	std::vector<std::unique_ptr<clk::output_port>> outputs;
};

inline auto constant_node::get_name() const -> std::string const&
{
	static std::string const name = "Constant";
	return name;
}

inline auto constant_node::get_output_ports() const -> std::vector<clk::output_port*>
{
	std::vector<clk::output_port*> ret;
	for(auto const& output : outputs)
		ret.push_back(output.get());
	return ret;
}

inline void constant_node::remove_port(clk::output_port* port)
{
	outputs.erase(std::remove_if(outputs.begin(), outputs.end(),
					  [&](auto& output) {
						  return output.get() == port;
					  }),
		outputs.end());
}

inline void constant_node::add_port(std::unique_ptr<clk::output_port>&& port)
{
	outputs.push_back(std::move(port));
}

} // namespace clk