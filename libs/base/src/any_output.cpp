#include "clk/base/any_output.hpp"
#include "clk/base/any_input.hpp"
#include "clk/base/input.hpp"

#include <memory>

namespace clk
{

any_output::~any_output()
{
	disconnect();
}

void any_output::set_data(void* data_pointer, std::size_t data_type_hash)
{
	if(data_pointer == nullptr || data_type_hash != 0)
	{
		assert(data_pointer != nullptr);
		assert(data_type_hash == 0);
		disconnect();
		// TODO only disconnect if not any_input
	}

	_data_pointer = data_pointer;
	_data_type_hash = data_type_hash;
}

void any_output::clear_data()
{
	set_data(nullptr, 0);
}

auto any_output::data_type_hash() const noexcept -> std::size_t
{
	return _data_type_hash;
}

auto any_output::data_pointer() const noexcept -> void const*
{
	return _data_pointer;
}

auto any_output::data_pointer() noexcept -> void*
{
	return _data_pointer;
}

auto any_output::can_connect_to(input const& other_input) const noexcept -> bool
{
	return other_input.data_type_hash() == _data_type_hash;
}

auto any_output::can_connect_to(port const& other_port) const noexcept -> bool
{
	if(auto const* other_input = dynamic_cast<input const*>(&other_port); other_input != nullptr)
	{
		return can_connect_to(*other_input);
	}
	return false;
}

void any_output::connect_to(input& other_input, bool notify)
{
	_connections.insert(&other_input);
	if(!other_input.is_connected_to(*this))
		other_input.connect_to(*this, notify);
	update_cached_connections();
	connection_changed();
}

void any_output::connect_to(port& other_port, bool notify)
{
	connect_to(dynamic_cast<input&>(other_port), notify);
}

void any_output::disconnect_from(input& other_input, bool notify)
{
	_connections.erase(&other_input);
	if(other_input.is_connected_to(*this))
		other_input.disconnect(notify);
	update_cached_connections();
	connection_changed();
}

void any_output::disconnect_from(port& other_port, bool notify)
{
	if(auto* other_input = dynamic_cast<input*>(&other_port); other_input != nullptr)
	{
		disconnect_from(*other_input, notify);
	}
}

void any_output::disconnect(bool notify)
{
	while(!_connections.empty())
		disconnect_from(**_connections.begin(), notify);
}

auto any_output::connected_ports() const -> std::vector<port*> const&
{
	return _cached_connected_ports;
}

auto any_output::connected_inputs() const -> std::vector<input*> const&
{
	return _cached_connected_inputs;
}

auto any_output::create_compatible_port() const -> std::unique_ptr<port>
{
	return std::make_unique<any_input>();
}

void any_output::update_cached_connections()
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