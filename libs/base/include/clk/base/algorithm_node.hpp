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

	auto get_name() const -> std::string const& final;
	void set_algorithm(std::unique_ptr<clk::algorithm>&& algorithm);
	auto get_inputs() const -> port_range<clk::input*> final;
	auto get_outputs() const -> port_range<clk::output*> final;
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

inline auto algorithm_node::get_name() const -> std::string const&
{
	static std::string const empty_name = "Empty algorithm node";
	if(_algorithm == nullptr)
		return empty_name;
	return _algorithm->get_name();
}

inline void algorithm_node::set_algorithm(std::unique_ptr<clk::algorithm>&& algorithm)
{
	_algorithm = std::move(algorithm);
	for(auto* port : _algorithm->get_inputs())
		port->set_push_callback([&](auto sentinel) {
			push(sentinel);
		});
	for(auto* port : _algorithm->get_outputs())
		port->set_pull_callback([&](auto sentinel) {
			pull(sentinel);
		});
}

inline auto algorithm_node::get_inputs() const -> port_range<clk::input*>
{
	if(_algorithm == nullptr)
		return {};
	return _algorithm->get_inputs();
}

inline auto algorithm_node::get_outputs() const -> port_range<clk::output*>
{
	if(_algorithm == nullptr)
		return {};
	return _algorithm->get_outputs();
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
	if(is_sink() || is_source())
		return true;

	return ranges::any_of(get_outputs(), [&](auto const* output) {
		return ranges::any_of(get_inputs(), [&output](auto const* input) {
			return input->get_timestamp() > output->get_timestamp();
		});
	});
}
} // namespace clk