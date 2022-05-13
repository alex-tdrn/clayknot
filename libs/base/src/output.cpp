#include "clk/base/output.hpp"

#include <utility>

namespace clk
{

void output::pull(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(_pull_callback)
		_pull_callback(sentinel);
}

void output::set_pull_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback)
{
	_pull_callback = callback;
}

void output::set_pull_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept
{
	_pull_callback = std::move(callback);
}
} // namespace clk