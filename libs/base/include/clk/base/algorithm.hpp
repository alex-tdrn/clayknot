#pragma once

#include "clk/base/input.hpp"
#include "clk/base/output.hpp"

#include <range/v3/algorithm.hpp>
#include <string>
#include <vector>

namespace clk
{
class algorithm
{
public:
	algorithm(const algorithm&) = delete;
	algorithm(algorithm&&) = delete;
	auto operator=(const algorithm&) = delete;
	auto operator=(algorithm&&) = delete;
	virtual ~algorithm() = default;

	virtual auto get_name() const noexcept -> std::string const& = 0;
	virtual void update() = 0;
	auto get_inputs() const noexcept -> std::vector<clk::input*> const&;
	auto get_outputs() const noexcept -> std::vector<clk::output*> const&;

protected:
	algorithm() = default;

	void register_port(clk::input& port);
	void register_port(clk::output& port);

private:
	std::vector<clk::input*> _inputs;
	std::vector<clk::output*> _outputs;
};

inline auto algorithm::get_inputs() const noexcept -> std::vector<clk::input*> const&
{
	return _inputs;
}

inline auto algorithm::get_outputs() const noexcept -> std::vector<clk::output*> const&
{
	return _outputs;
}

inline void algorithm::register_port(clk::input& input)
{
	if(ranges::find(_inputs, &input) != _inputs.end())
		return;
	_inputs.emplace_back(&input);
}

inline void algorithm::register_port(clk::output& output)
{
	if(ranges::find(_outputs, &output) != _outputs.end())
		return;
	_outputs.emplace_back(&output);
}
} // namespace clk