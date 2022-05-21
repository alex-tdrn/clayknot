#include "clk/base/algorithm_node.hpp"
#include "clk/base/input.hpp"
#include "clk/base/output.hpp"
#include "clk/base/sentinel.hpp"

#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <stdexcept>
#include <utility>
#include <vector>

namespace clk
{

algorithm_node::algorithm_node(std::unique_ptr<clk::algorithm>&& algorithm)
{
	set_algorithm(std::move(algorithm));
}

auto algorithm_node::name() const -> std::string_view
{
	if(_algorithm == nullptr)
		return "Empty algorithm node";
	return _algorithm->name();
}

void algorithm_node::set_algorithm(std::unique_ptr<clk::algorithm>&& algorithm)
{
	_algorithm = std::move(algorithm);
	try_update();
	for(auto* input : _algorithm->inputs())
		input->set_push_callback([&](auto sentinel) {
			push(sentinel);
		});
	for(auto* output : _algorithm->outputs())
		output->set_pull_callback([&](auto sentinel) {
			pull(sentinel);
		});
}

auto algorithm_node::inputs() const -> port_range<clk::input*>
{
	if(_algorithm == nullptr)
		return {};
	return _algorithm->inputs();
}

auto algorithm_node::outputs() const -> port_range<clk::output*>
{
	if(_algorithm == nullptr)
		return {};
	return _algorithm->outputs();
}

void algorithm_node::pull(std::weak_ptr<clk::sentinel> const& sentinel)
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

	node::pull(_sentinel);

	if(sentinel_origin != nullptr || update_needed())
		try_update();
}

void algorithm_node::push(std::weak_ptr<clk::sentinel> const& sentinel)
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

	node::pull(_sentinel);

	if(sentinel_origin != nullptr || update_needed())
		try_update();

	node::push(_sentinel);
}

auto algorithm_node::sentinel_present() const -> bool
{
	return !_sentinel.expired();
}

auto algorithm_node::update_possible() const -> bool
{
	return _algorithm != nullptr;
}

auto algorithm_node::update_needed() const -> bool
{
	if(!has_inputs() || !has_outputs())
		return true;

	return ranges::any_of(outputs(), [&](auto const* output) {
		return ranges::any_of(inputs(), [&output](auto const* input) {
			return input->timestamp() > output->timestamp();
		});
	});
}

void algorithm_node::try_update() const
{
	try
	{
		_algorithm->update();
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