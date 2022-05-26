#pragma once

#include "clk/base/port.hpp"
#include "clk/util/predicates.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <string_view>
#include <typeindex>
#include <unordered_set>
#include <variant>

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
	~output() override;

	using port::data_pointer;
	virtual auto data_pointer() noexcept -> void* = 0;

	auto can_connect_to(port const& other_port) const noexcept -> bool final;

	void connect_to(output& other_port) = delete;
	void connect_to(input& other_port, bool notify = true);
	void connect_to(port& other_port, bool notify = true) final;

	void disconnect_from(input& other_port, bool notify);
	void disconnect_from(port& other_port, bool notify = true) final;

	void disconnect(bool notify = true) final;

	auto connected_ports() const -> std::vector<port*> const& final;
	auto connected_inputs() const -> std::vector<input*> const&;

	void set_pull_callback(std::function<void(std::weak_ptr<clk::sentinel> const&)> callback) noexcept;
	void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) noexcept final;

private:
	std::function<void(std::weak_ptr<sentinel> const&)> _pull_callback;
	std::unordered_set<input*> _connections;
	std::vector<input*> _cached_connected_inputs;
	std::vector<port*> _cached_connected_ports;

	void update_cached_connections();
};

template <typename T>
class input_of;

template <typename T>
class output_of final : public output
{
public:
	using output::output;
	output_of(output_of const&) = delete;
	output_of(output_of&&) = delete;
	auto operator=(output_of const&) -> output_of& = delete;
	auto operator=(output_of&&) -> output_of& = delete;

	~output_of() final = default;

	auto data_type_hash() const noexcept -> std::size_t final
	{
		static std::size_t hash = std::type_index(typeid(T)).hash_code();
		return hash;
	}

	auto create_compatible_port() const -> std::unique_ptr<port> final
	{
		auto port = std::make_unique<input_of<T>>(name());
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
};

} // namespace clk