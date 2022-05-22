#include "clk/base/algorithm_node.hpp"

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
	for(auto* input : _algorithm->inputs())
		register_port(input);

	for(auto* output : _algorithm->outputs())
		register_port(output);

	pull();
}

auto algorithm_node::update_possible() const -> bool
{
	return _algorithm != nullptr;
}

void algorithm_node::update()
{
	_algorithm->update();
}

} // namespace clk