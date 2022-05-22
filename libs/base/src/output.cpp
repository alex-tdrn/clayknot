#include "clk/base/output.hpp"
#include "clk/base/input.hpp"

#include <utility>

namespace clk
{

void output::push(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	for(auto* connection : connected_inputs())
		connection->push(sentinel);
}

void output::pull(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(_pull_callback)
		_pull_callback(sentinel);
}

void output::set_pull_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)> callback) noexcept
{
	_pull_callback = std::move(callback);
}
} // namespace clk