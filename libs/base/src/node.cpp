#include "clk/base/node.hpp"
#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
#include "clk/base/sentinel.hpp"
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/algorithm/remove.hpp>

namespace clk
{

auto node::all_ports() const -> std::vector<clk::port*> const&
{
	return _ports;
}

auto node::inputs() const -> std::vector<clk::input*> const&
{
	return _inputs;
}

auto node::outputs() const -> std::vector<clk::output*> const&
{
	return _outputs;
}

void node::pull(std::weak_ptr<clk::sentinel> const& sentinel)
{
	if(sentinel_present() || !update_possible() || !error().empty())
		return;

	std::shared_ptr<clk::sentinel> sentinel_origin;
	if(sentinel.expired())
	{
		sentinel_origin = std::make_shared<clk::sentinel>();
		_sentinel = sentinel_origin;
	}
	else
	{
		_sentinel = sentinel;
	}

	pull_inputs(_sentinel);

	if(sentinel_origin != nullptr || update_needed())
		try_update();
}

void node::push(std::weak_ptr<clk::sentinel> const& sentinel)
{
	if(sentinel_present() || !update_possible())
		return;

	std::shared_ptr<clk::sentinel> sentinel_origin;
	if(sentinel.expired())
	{
		sentinel_origin = std::make_shared<clk::sentinel>();
		_sentinel = sentinel_origin;
	}
	else
	{
		_sentinel = sentinel;
	}

	clear_error();

	pull_inputs(sentinel);

	if(sentinel_origin != nullptr || update_needed())
		try_update();

	push_outputs(sentinel);
}

auto node::has_inputs() const -> bool
{
	return !_inputs.empty();
}

auto node::has_outputs() const -> bool
{
	return !_outputs.empty();
}

auto node::error() const -> std::string const&
{
	return _last_error_message;
}

void node::clear_error()
{
	_last_error_message.clear();

	for(auto* port : _outputs)
		port->mark_as_working();
}

void node::set_error(std::string_view error_message)
{
	_last_error_message = error_message;

	for(auto* port : _outputs)
		port->mark_as_faulty();
}

void node::register_port(clk::input* input)
{
	input->set_push_callback([&](auto sentinel) {
		push(sentinel);
	});

	_ports.push_back(input);
	_inputs.push_back(input);
}

void node::register_port(clk::output* output)
{
	output->set_pull_callback([&](auto sentinel) {
		pull(sentinel);
	});

	_ports.push_back(output);
	_outputs.push_back(output);
}

void node::unregister_port(clk::input* input)
{
	_ports.erase(ranges::remove(_ports, input), _ports.end());
	_inputs.erase(ranges::remove(_inputs, input), _inputs.end());
}

void node::unregister_port(clk::output* output)
{
	_ports.erase(ranges::remove(_ports, output), _ports.end());
	_outputs.erase(ranges::remove(_outputs, output), _outputs.end());
}

auto node::update_possible() const -> bool
{
	return true;
}

void node::update()
{
}

void node::pull_inputs(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* input_port : _inputs)
	{
		if(input_port->is_faulty())
		{
			for(auto* output_port : _outputs)
			{
				output_port->mark_as_faulty();
			}
			return;
		}
		input_port->pull(sentinel);
	}
}

void node::push_outputs(std::weak_ptr<clk::sentinel> const& sentinel)
{
	for(auto* output_port : _outputs)
		output_port->push(sentinel);
}

auto node::sentinel_present() const -> bool
{
	return !_sentinel.expired();
}

auto node::update_needed() const -> bool
{
	if(!has_inputs() || !has_outputs())
		return true;

	return ranges::any_of(outputs(), [&](auto const* output) {
		return ranges::any_of(inputs(), [&output](auto const* input) {
			return input->timestamp() > output->timestamp();
		});
	});
}

void node::try_update()
{
	try
	{
		update();
	}
	catch(const std::exception& e)
	{
		set_error(e.what());
	}
	catch(...)
	{
		set_error("Unknown error");
	}
}

} // namespace clk