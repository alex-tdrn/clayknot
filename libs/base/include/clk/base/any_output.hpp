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

	void set_data(void const* data_pointer, std::size_t data_type_hash);
	void clear_data();

	auto data_type_hash() const noexcept -> std::size_t final;
	auto data_pointer() const noexcept -> void const* final;
	auto data_pointer() noexcept -> void* final;

	auto create_compatible_port() const -> std::unique_ptr<port> final;

private:
	void const* _data_pointer = nullptr;
	std::size_t _data_type_hash = 0;
};
} // namespace clk