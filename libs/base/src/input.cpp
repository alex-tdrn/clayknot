#include "clk/base/input.hpp"

#include <utility>

namespace clk
{
void input::set_push_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback)
{
	_push_callback = callback;
}

void input::set_push_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept
{
	_push_callback = std::move(callback);
}

void input::push(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(_push_callback)
		_push_callback(sentinel);
}
} // namespace clk