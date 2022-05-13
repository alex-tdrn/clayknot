#pragma once

#include "clk/base/port.hpp"
#include "clk/util/predicates.hpp"
#include "clk/util/projections.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <typeindex>
#include <unordered_set>

namespace clk
{
class input;
class sentinel;

class output : public port
{
public:
	output(output const&) = delete;
	output(output&&) = delete;
	auto operator=(output const&) -> output& = delete;
	auto operator=(output&&) -> output& = delete;
	~output() override = default;

	using port::data_pointer;
	virtual auto data_pointer() noexcept -> void* = 0;
	using port::connect_to;
	void connect_to(output& other_port) = delete;
	virtual auto connected_inputs() const -> port_range<input*> = 0;
	void set_pull_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback);
	void set_pull_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;

protected:
	output() = default;

private:
	std::function<void(std::weak_ptr<sentinel> const&)> _pull_callback;
};

template <typename T>
class input_of;

template <typename T>
class output_of final : public output
{
public:
	using compatible_port = input_of<T>;

	output_of() = default;
	explicit output_of(std::string_view name);
	output_of(output_of const&) = delete;
	output_of(output_of&&) = delete;
	auto operator=(output_of const&) -> output_of& = delete;
	auto operator=(output_of&&) -> output_of& = delete;
	~output_of() final;

	auto data_pointer() const noexcept -> void const* final;
	auto data_pointer() noexcept -> void* final;
	auto data() noexcept -> T&;
	auto data() const noexcept -> T const&;
	auto operator*() noexcept -> T&;
	auto operator*() const noexcept -> T const&;
	auto operator->() noexcept -> T*;
	auto operator->() const noexcept -> T const*;
	auto data_type_hash() const noexcept -> std::size_t final;
	auto is_connected() const noexcept -> bool final;
	auto can_connect_to(port const& other_port) const noexcept -> bool final;
	auto is_connected_to(port const& other_port) const noexcept -> bool final;
	void connect_to(compatible_port& other_port, bool notify = true);
	void connect_to(port& other_port, bool notify = true) final;
	void disconnect_from(compatible_port& other_port, bool notify = true);
	void disconnect_from(port& other_port, bool notify = true) final;
	void disconnect(bool notify = true) final;
	auto connected_ports() const -> port_range<port*> final;
	auto connected_inputs() const -> port_range<input*> final;
	void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;
	auto create_compatible_port() const -> std::unique_ptr<port> final;

private:
	T _data = {};
	std::unordered_set<compatible_port*> _connections;
};

template <typename T>
output_of<T>::output_of(std::string_view name)
{
	set_name(name);
}

template <typename T>
output_of<T>::~output_of()
{
	disconnect();
}

template <typename T>
auto output_of<T>::data_type_hash() const noexcept -> std::size_t
{
	static std::size_t hash = std::type_index(typeid(T)).hash_code();
	return hash;
}

template <typename T>
auto output_of<T>::is_connected() const noexcept -> bool
{
	return !_connections.empty();
}

template <typename T>
auto output_of<T>::can_connect_to(port const& other_port) const noexcept -> bool
{
	return dynamic_cast<compatible_port const*>(&other_port);
}

template <typename T>
auto output_of<T>::is_connected_to(port const& other_port) const noexcept -> bool
{
	auto concrete = dynamic_cast<compatible_port const*>(&other_port);
	if(concrete == nullptr)
		return false;

	return ranges::any_of(_connections, clk::predicates::is_equal_to(concrete));
}

template <typename T>
void output_of<T>::connect_to(compatible_port& other_port, bool notify)
{
	_connections.insert(&other_port);
	if(!other_port.is_connected_to(*this))
		other_port.connect_to(*this, notify);

	connection_changed();
}

template <typename T>
void output_of<T>::connect_to(port& other_port, bool notify)
{
	connect_to(dynamic_cast<compatible_port&>(other_port), notify);
}

template <typename T>
void output_of<T>::disconnect_from(compatible_port& other_port, bool notify)
{
	_connections.erase(&other_port);
	if(other_port.is_connected_to(*this))
		other_port.disconnect(notify);

	connection_changed();
}

template <typename T>
void output_of<T>::disconnect_from(port& other_port, bool notify)
{
	auto concrete = dynamic_cast<compatible_port*>(&other_port);
	if(concrete)
		disconnect_from(*concrete, notify);
}

template <typename T>
void output_of<T>::disconnect(bool notify)
{
	while(!_connections.empty())
		disconnect_from(**_connections.begin(), notify);
}

template <typename T>
auto output_of<T>::connected_ports() const -> port_range<port*>
{
	return _connections;
}

template <typename T>
auto output_of<T>::connected_inputs() const -> port_range<input*>
{
	return _connections | ranges::views::transform(clk::projections::cast<input*>());
}

template <typename T>
void output_of<T>::push(std::weak_ptr<clk::sentinel> const& sentinel) noexcept
{
	for(auto connection : _connections)
		connection->push(sentinel);
}

template <typename T>
auto output_of<T>::create_compatible_port() const -> std::unique_ptr<port>
{
	auto port = std::make_unique<compatible_port>(name());
	if constexpr(std::is_copy_assignable_v<T>)
		port->default_port().data() = this->data();
	return port;
}

template <typename T>
auto output_of<T>::data_pointer() const noexcept -> void const*
{
	return &_data;
}

template <typename T>
auto output_of<T>::data_pointer() noexcept -> void*
{
	return &_data;
}

template <typename T>
auto output_of<T>::data() noexcept -> T&
{
	update_timestamp();
	return _data;
}

template <typename T>
auto output_of<T>::data() const noexcept -> T const&
{
	return _data;
}

template <typename T>
auto output_of<T>::operator*() noexcept -> T&
{
	update_timestamp();
	return data();
}

template <typename T>
auto output_of<T>::operator*() const noexcept -> T const&
{
	return data();
}

template <typename T>
auto output_of<T>::operator->() noexcept -> T*
{
	update_timestamp();
	return &_data;
}

template <typename T>
auto output_of<T>::operator->() const noexcept -> T const*
{
	return &_data;
}

} // namespace clk