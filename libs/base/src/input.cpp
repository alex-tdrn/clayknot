#include "clk/base/input.hpp"
#include "clk/base/output.hpp"

#include <utility>

namespace clk
{

input::~input()
{
	disconnect(false);
}

auto input::timestamp() const noexcept -> clk::timestamp
{
	if(auto* connection = connected_output(); connection != nullptr)
	{
		return std::max(connection->timestamp(), port::timestamp());
	}
	else
	{
		return std::max(default_port().timestamp(), port::timestamp());
	}
}

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

auto input::can_connect_to(port const& other_port) const noexcept -> bool
{
	if(auto const* other_output = dynamic_cast<output const*>(&other_port); other_output != nullptr)
	{
		return other_output->data_type_hash() == data_type_hash();
	}
	return false;
}

void input::connect_to(output& other_port, bool notify)
{
	if(&other_port == &default_port() || !can_connect_to(other_port))
	{
		return;
	}

	disconnect(false);
	_connection = &other_port;
	_cached_connected_ports = {_connection};
	if(!other_port.is_connected_to(*this))
		other_port.connect_to(*this, false);
	update_timestamp();
	connection_changed();
	if(notify)
		push();
}

void input::connect_to(port& other_port, bool notify)
{
	if(auto* other_output = dynamic_cast<output*>(&other_port); other_output != nullptr)
	{
		connect_to(*other_output, notify);
	}
}

void input::disconnect_from(port& other_port, bool notify)
{
	if(is_connected_to(other_port))
		disconnect(notify);
}

void input::disconnect(bool notify)
{
	if(_connection != nullptr)
	{
		auto* old_connection = _connection;
		_connection = nullptr;
		_cached_connected_ports.clear();
		old_connection->disconnect_from(*this, false);
		update_timestamp();
		connection_changed();
		if(notify)
			push();
	}
}

auto input::connected_ports() const -> std::vector<port*> const&
{
	return _cached_connected_ports;
}

auto input::connected_output() const -> output*
{
	return _connection;
}

void input::set_push_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)> callback) noexcept
{
	_push_callback = std::move(callback);
}

void input::push(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(_push_callback)
		_push_callback(sentinel);
}

void input::pull(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(auto* connection = connected_output(); connection != nullptr)
	{
		connection->pull(sentinel);
	}
}
} // namespace clk