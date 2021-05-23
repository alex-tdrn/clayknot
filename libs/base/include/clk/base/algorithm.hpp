#pragma once

#include "clk/base/port.hpp"

#include <string>
#include <vector>

namespace clk
{
class algorithm
{
public:
	algorithm(const algorithm&) = delete;
	algorithm(algorithm&&) = delete;
	auto operator=(const algorithm&) = delete;
	auto operator=(algorithm&&) = delete;
	virtual ~algorithm() = default;

	virtual auto get_name() const noexcept -> std::string const& = 0;
	virtual void update() = 0;
	auto get_input_ports() const noexcept -> std::vector<clk::input_port*> const&;
	auto get_output_ports() const noexcept -> std::vector<clk::output_port*> const&;

protected:
	algorithm() = default;

	void register_port(clk::input_port& port);
	void register_port(clk::output_port& port);

private:
	std::vector<clk::input_port*> _input_ports;
	std::vector<clk::output_port*> _output_ports;
};

inline auto algorithm::get_input_ports() const noexcept -> std::vector<clk::input_port*> const&
{
	return _input_ports;
}

inline auto algorithm::get_output_ports() const noexcept -> std::vector<clk::output_port*> const&
{
	return _output_ports;
}

inline void algorithm::register_port(clk::input_port& port)
{
	if(std::find(_input_ports.begin(), _input_ports.end(), &port) != _input_ports.end())
		return;
	_input_ports.emplace_back(&port);
}

inline void algorithm::register_port(clk::output_port& port)
{
	if(std::find(_output_ports.begin(), _output_ports.end(), &port) != _output_ports.end())
		return;
	_output_ports.emplace_back(&port);
}
} // namespace clk