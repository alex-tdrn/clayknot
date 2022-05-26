#include "clk/base/output.hpp"
#include "clk/base/input.hpp"

#include <range/v3/algorithm/any_of.hpp>
#include <stdexcept>
#include <utility>

namespace clk
{

output::~output()
{
	disconnect();
}

auto output::can_connect_to(port const& other_port) const noexcept -> bool
{
	if(auto const* other_input = dynamic_cast<input const*>(&other_port); other_input != nullptr)
	{
		return other_input->data_type_hash() == 0 || other_input->data_type_hash() == data_type_hash();
	}
	return false;
}

void output::connect_to(input& other_port, bool notify)
{
	if(!can_connect_to(other_port))
	{
		return;
	}

	_connections.insert(&other_port);
	if(!other_port.is_connected_to(*this))
		other_port.connect_to(*this, notify);
	update_cached_connections();
	connection_changed();
}

void output::connect_to(port& other_port, bool notify)
{
	if(auto* other_input = dynamic_cast<input*>(&other_port); other_input != nullptr)
	{
		connect_to(*other_input, notify);
	}
}

void output::disconnect_from(input& other_port, bool notify)
{
	_connections.erase(&other_port);
	if(other_port.is_connected_to(*this))
		other_port.disconnect(notify);
	update_cached_connections();
	connection_changed();
}

void output::disconnect_from(port& other_port, bool notify)
{
	if(auto* other_input = dynamic_cast<input*>(&other_port); other_input != nullptr)
	{
		disconnect_from(*other_input, notify);
	}
}

void output::disconnect(bool notify)
{
	while(!_connections.empty())
		disconnect_from(**_connections.begin(), notify);
}

auto output::connected_ports() const -> std::vector<port*> const&
{
	return _cached_connected_ports;
}

auto output::connected_inputs() const -> std::vector<input*> const&
{
	return _cached_connected_inputs;
}

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

void output::update_cached_connections()
{
	_cached_connected_inputs.clear();
	_cached_connected_ports.clear();
	for(auto* connection : _connections)
	{
		_cached_connected_ports.push_back(connection);
		_cached_connected_inputs.push_back(connection);
	}
}

} // namespace clk