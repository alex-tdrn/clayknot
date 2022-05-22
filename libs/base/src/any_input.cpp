#include "clk/base/any_input.hpp"
#include "clk/base/any_output.hpp"

#include <memory>

namespace clk
{
any_input::any_input()
{
	_default_port.connect_to(*this, false);
}

any_input::any_input(std::string_view name) : input(name)
{
	_default_port.connect_to(*this, false);
}

any_input::~any_input()
{
	disconnect(false);
}

auto any_input::data_type_hash() const noexcept -> std::size_t
{
	if(_connection != nullptr)
	{
		return _connection->data_type_hash();
	}
	return 0;
}

auto any_input::data_pointer() const noexcept -> void const*
{
	if(_connection != nullptr)
	{
		return _connection->data_pointer();
	}
	return nullptr;
}

auto any_input::can_connect_to(port const& other_port) const noexcept -> bool
{
	return dynamic_cast<output const*>(&other_port) != nullptr;
}

void any_input::connect_to(output& other_output, bool notify)
{
	if(&other_output == &_default_port)
		return;
	disconnect(false);
	_connection = &other_output;
	_cached_connected_ports = {_connection};
	if(!other_output.is_connected_to(*this))
		other_output.connect_to(*this, false);
	update_timestamp();
	connection_changed();
	if(notify)
		push();
}

void any_input::connect_to(port& other_port, bool notify)
{
	if(&other_port != &_default_port)
		connect_to(dynamic_cast<output&>(other_port), notify);
}

void any_input::disconnect_from(port& other_port, bool notify)
{
	if(is_connected_to(other_port))
		disconnect(notify);
}

void any_input::disconnect(bool notify)
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

auto any_input::connected_ports() const -> std::vector<port*> const&
{
	return _cached_connected_ports;
}

auto any_input::connected_output() const -> output*
{
	return _connection;
}

auto any_input::default_port() const -> output&
{
	return _default_port;
}

auto any_input::create_compatible_port() const -> std::unique_ptr<port>
{
	return std::make_unique<any_output>();
}

} // namespace clk