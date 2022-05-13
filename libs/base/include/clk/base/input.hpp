#pragma once

#include "clk/base/port.hpp"
#include "clk/util/timestamp.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <range/v3/view/single.hpp>
#include <string_view>
#include <typeindex>

namespace clk
{
class output;
class sentinel;

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

	input_of()
	{
		_default_port.connect_to(*this, false);
	}

	explicit input_of(std::string_view name)
	{
		set_name(name);
		_default_port.connect_to(*this, false);
	}

	input_of(input_of const&) = delete;
	input_of(input_of&&) = delete;
	auto operator=(input_of const&) -> input_of& = delete;
	auto operator=(input_of&&) -> input_of& = delete;

	~input_of() final
	{
		disconnect(false);
	}

	auto data_pointer() const noexcept -> void const* final
	{
		return &data();
	}

	auto data() const noexcept -> T const&
	{
		if(_connection)
			return _connection->data();
		return _default_port.data();
	}

	auto operator*() const noexcept -> T const&
	{
		return data();
	}

	auto operator->() const noexcept -> T const*
	{
		if(_connection)
			return _connection->operator->();
		return _default_port.operator->();
	}

	auto data_type_hash() const noexcept -> std::size_t final
	{
		static std::size_t hash = std::type_index(typeid(T)).hash_code();
		return hash;
	}

	auto timestamp() const noexcept -> clk::timestamp final
	{
		if(!_connection)
			return std::max(_default_port.timestamp(), port::timestamp());
		return std::max(_connection->timestamp(), port::timestamp());
	}

	auto is_connected() const noexcept -> bool final
	{
		return _connection != nullptr;
	}

	auto can_connect_to(port const& other_port) const noexcept -> bool final
	{
		return dynamic_cast<compatible_port const*>(&other_port);
	}

	auto is_connected_to(port const& other_port) const noexcept -> bool final
	{
		if(!_connection)
			return false;
		return _connection == dynamic_cast<compatible_port const*>(&other_port);
	}

	void connect_to(compatible_port& other_port, bool notify = true)
	{
		if(&other_port == &_default_port)
			return;
		disconnect(false);
		_connection = &other_port;
		if(!other_port.is_connected_to(*this))
			other_port.connect_to(*this, false);
		update_timestamp();
		connection_changed();
		if(notify)
			push();
	}

	void connect_to(port& other_port, bool notify = true) final
	{
		if(&other_port != &_default_port)
			connect_to(dynamic_cast<compatible_port&>(other_port), notify);
	}

	void disconnect(bool notify = true) final
	{
		if(_connection)
		{
			auto old_connection = _connection;
			_connection = nullptr;
			old_connection->disconnect_from(*this, false);
			update_timestamp();
			connection_changed();
			if(notify)
				push();
		}
	}

	void disconnect_from(port& other_port, bool notify = true) final
	{
		if(is_connected_to(other_port))
			disconnect(notify);
	}

	auto connected_ports() const -> port_range<port*> final
	{
		return ranges::views::single(_connection);
	}

	auto connected_output() const -> output* final
	{
		return _connection;
	}

	auto default_port() const -> compatible_port& final
	{
		return _default_port;
	}

	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final
	{
		if(_connection)
			_connection->pull(sentinel);
		else
			_default_port.pull(sentinel);
	}

	auto create_compatible_port() const -> std::unique_ptr<port> final
	{
		auto port = std::make_unique<compatible_port>(name());
		if constexpr(std::is_copy_assignable_v<T>)
			port->data() = this->data();
		return port;
	}

private:
	compatible_port mutable _default_port = compatible_port("Default port");
	compatible_port mutable* _connection = nullptr;
};

} // namespace clk