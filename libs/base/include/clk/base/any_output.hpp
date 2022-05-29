#pragma once

#include "clk/base/data.hpp"
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

	void set_data(const_data data);
	void clear_data();

	auto data_type_hash() const noexcept -> std::size_t final;
	auto abstract_data() const noexcept -> const_data final;
	auto abstract_data() noexcept -> mutable_data final;

	auto create_compatible_port() const -> std::unique_ptr<port> final;

private:
	const_data _data = {};
};
} // namespace clk