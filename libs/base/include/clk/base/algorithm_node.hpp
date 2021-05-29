#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/base/node.hpp"
#include "clk/base/port.hpp"
#include "clk/base/sentinel.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <range/v3/algorithm.hpp>
#include <string>
#include <vector>

namespace clk
{
class algorithm_node final : public node
{
public:
	algorithm_node() = default;
	explicit algorithm_node(std::unique_ptr<clk::algorithm>&& algorithm);
	algorithm_node(algorithm_node const&) = delete;
	algorithm_node(algorithm_node&&) noexcept = delete;
	auto operator=(algorithm_node const&) -> algorithm_node& = delete;
	auto operator=(algorithm_node&&) noexcept -> algorithm_node& = delete;
	~algorithm_node() final = default;

	auto name() const -> std::string_view final;
	void set_algorithm(std::unique_ptr<clk::algorithm>&& algorithm);
	auto inputs() const -> port_range<clk::input*> final;
	auto outputs() const -> port_range<clk::output*> final;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) final;
	void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) final;

private:
	std::unique_ptr<clk::algorithm> _algorithm;
	std::weak_ptr<clk::sentinel> _sentinel;

	auto sentinel_present() const -> bool;
	auto update_possible() const -> bool;
	auto update_needed() const -> bool;
};

inline algorithm_node::algorithm_node(std::unique_ptr<clk::algorithm>&& algorithm)
{
	set_algorithm(std::move(algorithm));
}

inline auto algorithm_node::name() const -> std::string_view
{
	if(_algorithm == nullptr)
		return "Empty algorithm node";
	return _algorithm->name();
}

inline void algorithm_node::set_algorithm(std::unique_ptr<clk::algorithm>&& algorithm)
{
	_algorithm = std::move(algorithm);
	_algorithm->update();
	for(auto* input : _algorithm->inputs())
		input->set_push_callback([&](auto sentinel) {
			push(sentinel);
		});
	for(auto* output : _algorithm->outputs())
		output->set_pull_callback([&](auto sentinel) {
			pull(sentinel);
		});
}

inline auto algorithm_node::inputs() const -> port_range<clk::input*>
{
	if(_algorithm == nullptr)
		return {};
	return _algorithm->inputs();
}

inline auto algorithm_node::outputs() const -> port_range<clk::output*>
{
	if(_algorithm == nullptr)
		return {};
	return _algorithm->outputs();
}

inline void algorithm_node::pull(std::weak_ptr<clk::sentinel> const& sentinel)
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

	node::pull(_sentinel);

	if(update_needed())
		_algorithm->update();
}

inline void algorithm_node::push(std::weak_ptr<clk::sentinel> const& sentinel)
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

	node::pull(_sentinel);

	if(update_needed())
		_algorithm->update();

	node::push(_sentinel);
}

inline auto algorithm_node::sentinel_present() const -> bool
{
	return !_sentinel.expired();
}

inline auto algorithm_node::update_possible() const -> bool
{
	return _algorithm != nullptr;
}

inline auto algorithm_node::update_needed() const -> bool
{
	if(!has_inputs() || !has_outputs())
		return true;

	return ranges::any_of(outputs(), [&](auto const* output) {
		return ranges::any_of(inputs(), [&output](auto const* input) {
			return input->timestamp() > output->timestamp();
		});
	});
}
} // namespace clk