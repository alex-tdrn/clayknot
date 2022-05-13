#include "clk/base/port.hpp"

namespace clk
{
void port::set_name(std::string_view name)
{
	_name = name;
}

auto port::name() const noexcept -> std::string_view
{
	return _name;
}

void port::update_timestamp() noexcept
{
	_timestamp.update();
}

void port::set_connection_changed_callback(std::function<void()> const& callback)
{
	_connection_changed_callback = callback;
}

auto port::timestamp() const noexcept -> clk::timestamp
{
	return _timestamp;
}

void port::connection_changed()
{
	if(_connection_changed_callback)
		_connection_changed_callback();
}
} // namespace clk