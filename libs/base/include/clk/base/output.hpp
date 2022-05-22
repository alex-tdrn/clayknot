#pragma once

#include "clk/base/port.hpp"
#include "clk/util/predicates.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <range/v3/algorithm/any_of.hpp>
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
	using port::port;
	output(output const&) = delete;
	output(output&&) = delete;
	auto operator=(output const&) -> output& = delete;
	auto operator=(output&&) -> output& = delete;
	~output() override = default;

	using port::data_pointer;
	virtual auto data_pointer() noexcept -> void* = 0;
	using port::connect_to;
	void connect_to(output& other_port) = delete;
	virtual auto connected_inputs() const -> std::vector<input*> const& = 0;
	void set_pull_callback(const std::function<void(std::weak_ptr<clk::sentinel> const&)>& callback);
	void set_pull_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)>&& callback) noexcept;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;

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

	using output::output;
	output_of(output_of const&) = delete;
	output_of(output_of&&) = delete;
	auto operator=(output_of const&) -> output_of& = delete;
	auto operator=(output_of&&) -> output_of& = delete;

	~output_of() final
	{
		disconnect();
	}

	auto data_type_hash() const noexcept -> std::size_t final
	{
		static std::size_t hash = std::type_index(typeid(T)).hash_code();
		return hash;
	}

	auto is_connected() const noexcept -> bool final
	{
		return !_connections.empty();
	}

	auto can_connect_to(port const& other_port) const noexcept -> bool final
	{
		return dynamic_cast<compatible_port const*>(&other_port);
	}

	auto is_connected_to(port const& other_port) const noexcept -> bool final
	{
		auto concrete = dynamic_cast<compatible_port const*>(&other_port);
		if(concrete == nullptr)
			return false;

		return ranges::any_of(_connections, clk::predicates::is_equal_to(concrete));
	}

	void connect_to(compatible_port& other_port, bool notify = true)
	{
		_connections.insert(&other_port);
		if(!other_port.is_connected_to(*this))
			other_port.connect_to(*this, notify);
		update_cached_connections();
		connection_changed();
	}

	void connect_to(port& other_port, bool notify = true) final
	{
		connect_to(dynamic_cast<compatible_port&>(other_port), notify);
	}

	void disconnect_from(compatible_port& other_port, bool notify)
	{
		_connections.erase(&other_port);
		if(other_port.is_connected_to(*this))
			other_port.disconnect(notify);
		update_cached_connections();
		connection_changed();
	}

	void disconnect_from(port& other_port, bool notify = true) final
	{
		auto concrete = dynamic_cast<compatible_port*>(&other_port);
		if(concrete)
			disconnect_from(*concrete, notify);
	}

	void disconnect(bool notify = true) final
	{
		while(!_connections.empty())
			disconnect_from(**_connections.begin(), notify);
	}

	auto connected_ports() const -> std::vector<port*> const& final
	{
		return _cached_connected_ports;
	}

	auto connected_inputs() const -> std::vector<input*> const& final
	{
		return _cached_connected_inputs;
	}

	void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final
	{
		for(auto connection : _connections)
			connection->push(sentinel);
	}

	auto create_compatible_port() const -> std::unique_ptr<port> final
	{
		auto port = std::make_unique<compatible_port>(name());
		if constexpr(std::is_copy_assignable_v<T>)
			port->default_port().data() = this->data();
		return port;
	}

	auto data_pointer() const noexcept -> void const* final
	{
		return &_data;
	}

	auto data_pointer() noexcept -> void* final
	{
		return &_data;
	}

	auto data() noexcept -> T&
	{
		update_timestamp();
		return _data;
	}

	auto data() const noexcept -> T const&
	{
		return _data;
	}

	auto operator*() noexcept -> T&
	{
		update_timestamp();
		return data();
	}

	auto operator*() const noexcept -> T const&
	{
		return data();
	}

	auto operator->() noexcept -> T*
	{
		update_timestamp();
		return &_data;
	}

	auto operator->() const noexcept -> T const*
	{
		return &_data;
	}

private:
	T _data = {};
	std::unordered_set<compatible_port*> _connections;
	std::vector<input*> _cached_connected_inputs;
	std::vector<port*> _cached_connected_ports;

	void update_cached_connections()
	{
		_cached_connected_inputs.clear();
		_cached_connected_ports.clear();
		for(auto* connection : _connections)
		{
			_cached_connected_ports.push_back(connection);
			_cached_connected_inputs.push_back(connection);
		}
	}
};

} // namespace clk