#pragma once

#include "clk/base/input.hpp"

#include "clk/base/any_output.hpp"

namespace clk
{
class output;

class any_input final : public input
{
public:
	any_input();
	explicit any_input(std::string_view name);

	any_input(any_input const&) = delete;
	any_input(any_input&&) = delete;
	auto operator=(any_input const&) -> any_input& = delete;
	auto operator=(any_input&&) -> any_input& = delete;
	~any_input() final;

	auto data_type_hash() const noexcept -> std::size_t final;
	auto data_pointer() const noexcept -> void const* final;

	auto can_connect_to(port const& other_port) const noexcept -> bool final;

	void connect_to(output& other_output, bool notify = true);
	void connect_to(port& other_port, bool notify = true) final;

	void disconnect_from(port& other_port, bool notify = true) final;

	void disconnect(bool notify = true) final;

	auto connected_ports() const -> std::vector<port*> const& final;
	auto connected_output() const -> output* final;
	auto default_port() const -> output& final;
	auto create_compatible_port() const -> std::unique_ptr<port> final;

private:
	any_output mutable _default_port{"Default port"};
	output* _connection = nullptr;
	std::vector<port*> _cached_connected_ports = {};
};

} // namespace clk