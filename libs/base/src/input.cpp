#include "clk/base/input.hpp"
#include "clk/base/output.hpp"

#include <utility>

namespace clk
{
auto input::is_faulty() const noexcept -> bool
{
	if(auto* connection = connected_output(); connection != nullptr)
	{
		return connection->is_faulty() || port::is_faulty();
	}
	else
	{
		return default_port().is_faulty() || port::is_faulty();
	}
}

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