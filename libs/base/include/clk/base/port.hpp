#pragma once

#include "clk/base/sentinel.hpp"
#include "clk/util/timestamp.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>

namespace clk
{
class port
{
public:
	port(port&&) = delete;
	port(port const&) = delete;
	auto operator=(port&&) -> port& = delete;
	auto operator=(port const&) -> port& = delete;
	virtual ~port() = default;

	void set_name(const std::string& name);
	void set_name(std::string&& name) noexcept;
	auto get_name() const noexcept -> std::string const&;
	void update_timestamp() noexcept;
	virtual auto get_timestamp() const noexcept -> clk::timestamp;
	virtual auto get_data_pointer() const noexcept -> void const* = 0;
	virtual auto get_data_type_hash() const noexcept -> std::size_t = 0;
	virtual auto is_connected() const noexcept -> bool = 0;
	virtual auto can_connect_to(port const& other_port) const noexcept -> bool = 0;
	virtual auto is_connected_to(port const& other_port) const noexcept -> bool = 0;
	virtual void connect_to(port& other_port, bool notify = true) = 0;
	virtual void disconnect_from(port& other_port, bool notify = true) = 0;
	virtual void disconnect(bool notify = true) = 0;
	virtual auto get_connected_ports() const -> std::unordered_set<port*> = 0;
	virtual void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept = 0;
	virtual void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept = 0;
	virtual auto create_compatible_port() const -> std::unique_ptr<port> = 0;

protected:
	port() = default;

private:
	std::string name = "Unnamed";
	clk::timestamp timestamp;
};

class output_port;

class input_port : public port
{
public:
	input_port(input_port const&) = delete;
	input_port(input_port&&) = delete;
	auto operator=(input_port const&) -> input_port& = delete;
	auto operator=(input_port&&) -> input_port& = delete;
	~input_port() override = default;

	using port::connect_to;
	void connect_to(input_port& other_port) = delete;
	virtual auto get_connected_output_port() const -> output_port* = 0;
	virtual auto get_default_port() const -> output_port& = 0;
	void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;
	void set_push_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback);
	void set_push_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept;

protected:
	input_port() = default;

private:
	std::function<void(std::weak_ptr<clk::sentinel> const&)> push_callback;
};

class output_port : public port
{
public:
	output_port(output_port const&) = delete;
	output_port(output_port&&) = delete;
	auto operator=(output_port const&) -> output_port& = delete;
	auto operator=(output_port&&) -> output_port& = delete;
	~output_port() override = default;

	using port::get_data_pointer;
	virtual auto get_data_pointer() noexcept -> void* = 0;
	using port::connect_to;
	void connect_to(output_port& other_port) = delete;
	virtual auto get_connected_input_ports() const -> std::unordered_set<input_port*> = 0;
	void set_pull_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback);
	void set_pull_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;

protected:
	output_port() = default;

private:
	std::function<void(std::weak_ptr<sentinel> const&)> pull_callback;
};

template <typename T>
class input_port_of;

template <typename T>
class output_port_of final : public output_port
{
public:
	using compatible_port = input_port_of<T>;

	output_port_of() = default;
	explicit output_port_of(std::string name);
	output_port_of(output_port_of const&) = delete;
	output_port_of(output_port_of&&) = delete;
	auto operator=(output_port_of const&) -> output_port_of& = delete;
	auto operator=(output_port_of&&) -> output_port_of& = delete;
	~output_port_of() final;

	auto get_data_pointer() const noexcept -> void const* final;
	auto get_data_pointer() noexcept -> void* final;
	auto get() noexcept -> T&;
	auto get() const noexcept -> T const&;
	auto operator*() noexcept -> T&;
	auto operator*() const noexcept -> T const&;
	auto operator->() noexcept -> T*;
	auto operator->() const noexcept -> T const*;
	auto get_data_type_hash() const noexcept -> std::size_t final;
	auto is_connected() const noexcept -> bool final;
	auto can_connect_to(port const& other_port) const noexcept -> bool final;
	auto is_connected_to(port const& other_port) const noexcept -> bool final;
	void connect_to(compatible_port& other_port, bool notify = true);
	void connect_to(port& other_port, bool notify = true) final;
	void disconnect_from(compatible_port& other_port, bool notify = true);
	void disconnect_from(port& other_port, bool notify = true) final;
	void disconnect(bool notify = true) final;
	auto get_connected_ports() const -> std::unordered_set<port*> final;
	auto get_connected_input_ports() const -> std::unordered_set<input_port*> final;
	void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;
	auto create_compatible_port() const -> std::unique_ptr<port> final;

private:
	T data = {};
	std::unordered_set<compatible_port*> connections;
};

template <typename T>
class input_port_of final : public input_port
{
public:
	using compatible_port = output_port_of<T>;

	input_port_of();
	explicit input_port_of(std::string name);
	input_port_of(input_port_of const&) = delete;
	input_port_of(input_port_of&&) = delete;
	auto operator=(input_port_of const&) -> input_port_of& = delete;
	auto operator=(input_port_of&&) -> input_port_of& = delete;
	~input_port_of() final;

	auto get_data_pointer() const noexcept -> void const* final;
	auto get() const noexcept -> T const&;
	auto operator*() const noexcept -> T const&;
	auto operator->() const noexcept -> T const*;
	auto get_data_type_hash() const noexcept -> std::size_t final;
	auto get_timestamp() const noexcept -> clk::timestamp final;
	auto is_connected() const noexcept -> bool final;
	auto can_connect_to(port const& other_port) const noexcept -> bool final;
	auto is_connected_to(port const& other_port) const noexcept -> bool final;
	void connect_to(compatible_port& other_port, bool notify = true);
	void connect_to(port& other_port, bool notify = true) final;
	void disconnect(bool notify = true) final;
	void disconnect_from(port& other_port, bool notify = true) final;
	auto get_connected_ports() const -> std::unordered_set<port*> final;
	auto get_connected_output_port() const -> output_port* final;
	auto get_default_port() const -> compatible_port& final;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;
	auto create_compatible_port() const -> std::unique_ptr<port> final;

private:
	compatible_port mutable default_port = compatible_port("Default port");
	compatible_port mutable* connection = nullptr;
};

inline void port::set_name(const std::string& name)
{
	this->name = name;
}

inline void port::set_name(std::string&& name) noexcept
{
	this->name = std::move(name);
}

inline auto port::get_name() const noexcept -> std::string const&
{
	return name;
}

inline void port::update_timestamp() noexcept
{
	timestamp.update();
}

inline auto port::get_timestamp() const noexcept -> clk::timestamp
{
	return timestamp;
}

inline void input_port::set_push_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback)
{
	push_callback = callback;
}

inline void input_port::set_push_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept
{
	push_callback = std::move(callback);
}

inline void input_port::push(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(push_callback)
		push_callback(sentinel);
}

inline void output_port::pull(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(pull_callback)
		pull_callback(sentinel);
}

inline void output_port::set_pull_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback)
{
	pull_callback = callback;
}

inline void output_port::set_pull_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept
{
	pull_callback = std::move(callback);
}

template <typename T>
output_port_of<T>::output_port_of(std::string name)
{
	set_name(std::move(name));
}

template <typename T>
output_port_of<T>::~output_port_of()
{
	disconnect();
}

template <typename T>
auto output_port_of<T>::get_data_type_hash() const noexcept -> std::size_t
{
	static std::size_t hash = std::type_index(typeid(T)).hash_code();
	return hash;
}

template <typename T>
auto output_port_of<T>::is_connected() const noexcept -> bool
{
	return !connections.empty();
}

template <typename T>
auto output_port_of<T>::can_connect_to(port const& other_port) const noexcept -> bool
{
	return dynamic_cast<compatible_port const*>(&other_port);
}

template <typename T>
auto output_port_of<T>::is_connected_to(port const& other_port) const noexcept -> bool
{
	auto concrete = dynamic_cast<compatible_port const*>(&other_port);
	if(concrete == nullptr)
		return false;

	return std::any_of(connections.begin(), connections.end(), [&concrete](auto const* port) {
		return port == concrete;
	});
}

template <typename T>
void output_port_of<T>::connect_to(compatible_port& other_port, bool notify)
{
	connections.insert(&other_port);
	if(!other_port.is_connected_to(*this))
		other_port.connect_to(*this, notify);
}

template <typename T>
void output_port_of<T>::connect_to(port& other_port, bool notify)
{
	connect_to(dynamic_cast<compatible_port&>(other_port), notify);
}

template <typename T>
void output_port_of<T>::disconnect_from(compatible_port& other_port, bool notify)
{
	connections.erase(&other_port);
	if(other_port.is_connected_to(*this))
		other_port.disconnect(notify);
}

template <typename T>
void output_port_of<T>::disconnect_from(port& other_port, bool notify)
{
	auto concrete = dynamic_cast<compatible_port*>(&other_port);
	if(concrete)
		disconnect_from(*concrete, notify);
}

template <typename T>
void output_port_of<T>::disconnect(bool notify)
{
	while(!connections.empty())
		disconnect_from(**connections.begin(), notify);
}

template <typename T>
auto output_port_of<T>::get_connected_ports() const -> std::unordered_set<port*>
{
	std::unordered_set<port*> connections;
	for(auto connection : this->connections)
		connections.insert(connection);
	return connections;
}

template <typename T>
auto output_port_of<T>::get_connected_input_ports() const -> std::unordered_set<input_port*>
{
	std::unordered_set<input_port*> connections;
	for(auto connection : this->connections)
		connections.insert(connection);
	return connections;
}

template <typename T>
void output_port_of<T>::push(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	for(auto connection : connections)
		connection->push(sentinel);
}

template <typename T>
auto output_port_of<T>::create_compatible_port() const -> std::unique_ptr<port>
{
	auto port = std::make_unique<compatible_port>(get_name());
	if constexpr(std::is_copy_assignable_v<T>)
		port->get_default_port().get() = this->get();
	return port;
}

template <typename T>
auto output_port_of<T>::get_data_pointer() const noexcept -> void const*
{
	return &data;
}

template <typename T>
auto output_port_of<T>::get_data_pointer() noexcept -> void*
{
	return &data;
}

template <typename T>
auto output_port_of<T>::get() noexcept -> T&
{
	update_timestamp();
	return data;
}

template <typename T>
auto output_port_of<T>::get() const noexcept -> T const&
{
	return data;
}

template <typename T>
auto output_port_of<T>::operator*() noexcept -> T&
{
	update_timestamp();
	return get();
}

template <typename T>
auto output_port_of<T>::operator*() const noexcept -> T const&
{
	return get();
}

template <typename T>
auto output_port_of<T>::operator->() noexcept -> T*
{
	update_timestamp();
	return &data;
}

template <typename T>
auto output_port_of<T>::operator->() const noexcept -> T const*
{
	return &data;
}

template <typename T>
input_port_of<T>::input_port_of()
{
	default_port.connect_to(*this, false);
}

template <typename T>
input_port_of<T>::input_port_of(std::string name)
{
	set_name(std::move(name));
	default_port.connect_to(*this, false);
}

template <typename T>
input_port_of<T>::~input_port_of()
{
	disconnect(false);
}

template <typename T>
auto input_port_of<T>::get_data_type_hash() const noexcept -> std::size_t
{
	static std::size_t hash = std::type_index(typeid(T)).hash_code();
	return hash;
}

template <typename T>
auto input_port_of<T>::get_timestamp() const noexcept -> clk::timestamp
{
	if(!connection)
		return std::max(default_port.get_timestamp(), port::get_timestamp());
	return std::max(connection->get_timestamp(), port::get_timestamp());
}

template <typename T>
auto input_port_of<T>::is_connected() const noexcept -> bool
{
	return connection != nullptr;
}

template <typename T>
auto input_port_of<T>::can_connect_to(port const& other_port) const noexcept -> bool
{
	return dynamic_cast<compatible_port const*>(&other_port);
}

template <typename T>
auto input_port_of<T>::is_connected_to(port const& other_port) const noexcept -> bool
{
	if(!connection)
		return false;
	return connection == dynamic_cast<compatible_port const*>(&other_port);
}

template <typename T>
void input_port_of<T>::connect_to(compatible_port& other_port, bool notify)
{
	if(&other_port == &default_port)
		return;
	disconnect(false);
	connection = &other_port;
	if(!other_port.is_connected_to(*this))
		other_port.connect_to(*this, false);
	update_timestamp();
	if(notify)
		push();
}

template <typename T>
void input_port_of<T>::connect_to(port& other_port, bool notify)
{
	if(&other_port != &default_port)
		connect_to(dynamic_cast<compatible_port&>(other_port), notify);
}

template <typename T>
void input_port_of<T>::disconnect(bool notify)
{
	if(connection)
	{
		auto old_connection = connection;
		connection = nullptr;
		old_connection->disconnect_from(*this, false);
		update_timestamp();
		if(notify)
			push();
	}
}

template <typename T>
void input_port_of<T>::disconnect_from(port& other_port, bool notify)
{
	if(is_connected_to(other_port))
		disconnect(notify);
}

template <typename T>
auto input_port_of<T>::get_connected_ports() const -> std::unordered_set<port*>
{
	std::unordered_set<port*> connections;
	if(connection != nullptr)
		connections.insert(connection);
	return connections;
}

template <typename T>
auto input_port_of<T>::get_connected_output_port() const -> output_port*
{
	return connection;
}

template <typename T>
auto input_port_of<T>::get_default_port() const -> compatible_port&
{
	return default_port;
}

template <typename T>
void input_port_of<T>::pull(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	if(connection)
		connection->pull(sentinel);
	else
		default_port.pull(sentinel);
}

template <typename T>
auto input_port_of<T>::create_compatible_port() const -> std::unique_ptr<port>
{
	auto port = std::make_unique<compatible_port>(get_name());
	if constexpr(std::is_copy_assignable_v<T>)
		port->get() = this->get();
	return port;
}

template <typename T>
auto input_port_of<T>::get_data_pointer() const noexcept -> void const*
{
	return &get();
}

template <typename T>
auto input_port_of<T>::get() const noexcept -> T const&
{
	if(connection)
		return connection->get();
	return default_port.get();
}

template <typename T>
auto input_port_of<T>::operator*() const noexcept -> T const&
{
	return get();
}

template <typename T>
auto input_port_of<T>::operator->() const noexcept -> T const*
{
	if(connection)
		return connection->operator->();
	return default_port.operator->();
}

} // namespace clk