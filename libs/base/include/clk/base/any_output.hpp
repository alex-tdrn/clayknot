#pragma once

#include "clk/base/output.hpp"

namespace clk
{

class input;

class any_output final : public output
{
public:
	using output::output;
	any_output(any_output const&) = delete;
	any_output(any_output&&) = delete;
	auto operator=(any_output const&) -> any_output& = delete;
	auto operator=(any_output&&) -> any_output& = delete;
	~any_output() final;

	void set_data(void* data_pointer, std::size_t data_type_hash);
	void clear_data();

	auto data_type_hash() const noexcept -> std::size_t final;
	auto data_pointer() const noexcept -> void const* final;
	auto data_pointer() noexcept -> void* final;

	auto can_connect_to(input const& other_input) const noexcept -> bool;
	auto can_connect_to(port const& other_port) const noexcept -> bool final;

	void connect_to(input& other_input, bool notify = true);
	void connect_to(port& other_port, bool notify = true) final;

	void disconnect_from(input& other_input, bool notify = true);
	void disconnect_from(port& other_port, bool notify = true) final;

	void disconnect(bool notify = true) final;

	auto connected_ports() const -> std::vector<port*> const& final;
	auto connected_inputs() const -> std::vector<input*> const& final;
	auto create_compatible_port() const -> std::unique_ptr<port> final;

private:
	void* _data_pointer = nullptr;
	std::size_t _data_type_hash = 0;
	std::unordered_set<input*> _connections;
	std::vector<input*> _cached_connected_inputs;
	std::vector<port*> _cached_connected_ports;

	void update_cached_connections();
};
} // namespace clk