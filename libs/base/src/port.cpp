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

void port::mark_as_faulty() const noexcept
{
	_faulty = true;
}

void port::mark_as_working() const noexcept
{
	_faulty = false;
}

void port::set_connection_changed_callback(std::function<void()> const& callback)
{
	_connection_changed_callback = callback;
}

auto port::timestamp() const noexcept -> clk::timestamp
{
	return _timestamp;
}

auto port::is_faulty() const noexcept -> bool
{
	return _faulty;
}

void port::connection_changed()
{
	if(_connection_changed_callback)
		_connection_changed_callback();
}
} // namespace clk