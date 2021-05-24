#pragma once

#include "clk/base/port.hpp"

#include <functional>
#include <typeindex>

namespace clk
{
class output;

class input : public port
{
public:
	input(input const&) = delete;
	input(input&&) = delete;
	auto operator=(input const&) -> input& = delete;
	auto operator=(input&&) -> input& = delete;
	~input() override = default;

	using port::connect_to;
	void connect_to(input& other_port) = delete;
	virtual auto connected_output() const -> output* = 0;
	virtual auto default_port() const -> output& = 0;
	void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;
	void set_push_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback);
	void set_push_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept;

protected:
	input() = default;

private:
	std::function<void(std::weak_ptr<clk::sentinel> const&)> _push_callback;
};

template <typename T>
class output_of;

template <typename T>
class input_of final : public input
{
public:
	using compatible_port = output_of<T>;

	input_of();
	explicit input_of(std::string name);
	input_of(input_of const&) = delete;
	input_of(input_of&&) = delete;
	auto operator=(input_of const&) -> input_of& = delete;
	auto operator=(input_of&&) -> input_of& = delete;
	~input_of() final;

	auto data_pointer() const noexcept -> void const* final;
	auto data() const noexcept -> T const&;
	auto operator*() const noexcept -> T const&;
	auto operator->() const noexcept -> T const*;
	auto data_type_hash() const noexcept -> std::size_t final;
	auto timestamp() const noexcept -> clk::timestamp final;
	auto is_connected() const noexcept -> bool final;
	auto can_connect_to(port const& other_port) const noexcept -> bool final;
	auto is_connected_to(port const& other_port) const noexcept -> bool final;
	void connect_to(compatible_port& other_port, bool notify = true);
	void connect_to(port& other_port, bool notify = true) final;
	void disconnect(bool notify = true) final;
	void disconnect_from(port& other_port, bool notify = true) final;
	auto connected_ports() const -> port_range<port*> final;
	auto connected_output() const -> output* final;
	auto default_port() const -> compatible_port& final;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;
	auto create_compatible_port() const -> std::unique_ptr<port> final;

private:
	compatible_port mutable _default_port = compatible_port("Default port");
	compatible_port mutable* _connection = nullptr;
};

inline void input::set_push_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback)
{
	_push_callback = callback;
}

inline void input::set_push_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept
{
	_push_callback = std::move(callback);
}

inline void input::push(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(_push_callback)
		_push_callback(sentinel);
}

template <typename T>
input_of<T>::input_of()
{
	_default_port.connect_to(*this, false);
}

template <typename T>
input_of<T>::input_of(std::string name)
{
	set_name(std::move(name));
	_default_port.connect_to(*this, false);
}

template <typename T>
input_of<T>::~input_of()
{
	disconnect(false);
}

template <typename T>
auto input_of<T>::data_type_hash() const noexcept -> std::size_t
{
	static std::size_t hash = std::type_index(typeid(T)).hash_code();
	return hash;
}

template <typename T>
auto input_of<T>::timestamp() const noexcept -> clk::timestamp
{
	if(!_connection)
		return std::max(_default_port.timestamp(), port::timestamp());
	return std::max(_connection->timestamp(), port::timestamp());
}

template <typename T>
auto input_of<T>::is_connected() const noexcept -> bool
{
	return _connection != nullptr;
}

template <typename T>
auto input_of<T>::can_connect_to(port const& other_port) const noexcept -> bool
{
	return dynamic_cast<compatible_port const*>(&other_port);
}

template <typename T>
auto input_of<T>::is_connected_to(port const& other_port) const noexcept -> bool
{
	if(!_connection)
		return false;
	return _connection == dynamic_cast<compatible_port const*>(&other_port);
}

template <typename T>
void input_of<T>::connect_to(compatible_port& other_port, bool notify)
{
	if(&other_port == &_default_port)
		return;
	disconnect(false);
	_connection = &other_port;
	if(!other_port.is_connected_to(*this))
		other_port.connect_to(*this, false);
	update_timestamp();
	if(notify)
		push();
}

template <typename T>
void input_of<T>::connect_to(port& other_port, bool notify)
{
	if(&other_port != &_default_port)
		connect_to(dynamic_cast<compatible_port&>(other_port), notify);
}

template <typename T>
void input_of<T>::disconnect(bool notify)
{
	if(_connection)
	{
		auto old_connection = _connection;
		_connection = nullptr;
		old_connection->disconnect_from(*this, false);
		update_timestamp();
		if(notify)
			push();
	}
}

template <typename T>
void input_of<T>::disconnect_from(port& other_port, bool notify)
{
	if(is_connected_to(other_port))
		disconnect(notify);
}

template <typename T>
auto input_of<T>::connected_ports() const -> port_range<port*>
{
	return ranges::views::single(_connection);
}

template <typename T>
auto input_of<T>::connected_output() const -> output*
{
	return _connection;
}

template <typename T>
auto input_of<T>::default_port() const -> compatible_port&
{
	return _default_port;
}

template <typename T>
void input_of<T>::pull(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(_connection)
		_connection->pull(sentinel);
	else
		_default_port.pull(sentinel);
}

template <typename T>
auto input_of<T>::create_compatible_port() const -> std::unique_ptr<port>
{
	auto port = std::make_unique<compatible_port>(name());
	if constexpr(std::is_copy_assignable_v<T>)
		port->data() = this->data();
	return port;
}

template <typename T>
auto input_of<T>::data_pointer() const noexcept -> void const*
{
	return &data();
}

template <typename T>
auto input_of<T>::data() const noexcept -> T const&
{
	if(_connection)
		return _connection->data();
	return _default_port.data();
}

template <typename T>
auto input_of<T>::operator*() const noexcept -> T const&
{
	return data();
}

template <typename T>
auto input_of<T>::operator->() const noexcept -> T const*
{
	if(_connection)
		return _connection->operator->();
	return _default_port.operator->();
}

} // namespace clk