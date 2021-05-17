#pragma once

#include "clk/base/algorithm.hpp"
#include "clk/base/node.hpp"
#include "clk/base/port.hpp"
#include "clk/base/sentinel.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
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
	auto get_input_ports() const -> std::vector<clk::input_port*> final;
	auto get_output_ports() const -> std::vector<clk::output_port*> final;
	void pull(std::weak_ptr<clk::sentinel> const& sentinel = {}) final;
	void push(std::weak_ptr<clk::sentinel> const& sentinel = {}) final;

private:
	std::unique_ptr<clk::algorithm> algorithm;
	std::weak_ptr<clk::sentinel> sentinel;

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
	if(algorithm == nullptr)
		return empty_name;
	return algorithm->get_name();
}

inline void algorithm_node::set_algorithm(std::unique_ptr<clk::algorithm>&& algorithm)
{
	this->algorithm = std::move(algorithm);
	for(auto* port : this->algorithm->get_input_ports())
		port->set_push_callback([&](auto sentinel) {
			this->push(sentinel);
		});
	for(auto* port : this->algorithm->get_output_ports())
		port->set_pull_callback([&](auto sentinel) {
			this->pull(sentinel);
		});
}

inline auto algorithm_node::get_input_ports() const -> std::vector<clk::input_port*>
{
	if(algorithm == nullptr)
		return {};
	return algorithm->get_input_ports();
}

inline auto algorithm_node::get_output_ports() const -> std::vector<clk::output_port*>
{
	if(algorithm == nullptr)
		return {};
	return algorithm->get_output_ports();
}

inline void algorithm_node::pull(std::weak_ptr<clk::sentinel> const& sentinel)
{
	if(sentinel_present() || !update_possible())
		return;

	std::shared_ptr<clk::sentinel> sentinel_origin;
	if(sentinel.expired())
	{
		sentinel_origin = std::make_shared<clk::sentinel>();
		this->sentinel = sentinel_origin;
	}
	else
	{
		this->sentinel = sentinel;
	}

	node::pull(sentinel);

	if(update_needed())
		algorithm->update();
}

inline void algorithm_node::push(std::weak_ptr<clk::sentinel> const& sentinel)
{
	if(sentinel_present() || !update_possible())
		return;

	std::shared_ptr<clk::sentinel> sentinel_origin;
	if(sentinel.expired())
	{
		sentinel_origin = std::make_shared<clk::sentinel>();
		this->sentinel = sentinel_origin;
	}
	else
	{
		this->sentinel = sentinel;
	}

	node::pull(sentinel);

	if(update_needed())
		algorithm->update();

	node::push(sentinel);
}

inline auto algorithm_node::sentinel_present() const -> bool
{
	return !sentinel.expired();
}

inline auto algorithm_node::update_possible() const -> bool
{
	return algorithm != nullptr;
}

inline auto algorithm_node::update_needed() const -> bool
{
	if(get_input_ports().empty())
		return true;
	if(get_output_ports().empty())
		return true;

	auto outputs = get_output_ports();
	auto inputs = get_input_ports();

	return std::any_of(outputs.begin(), outputs.end(), [&](auto const* output_port) {
		return std::any_of(inputs.begin(), inputs.end(), [&output_port](auto const* input_port) {
			return input_port->get_timestamp() > output_port->get_timestamp();
		});
	});
}
} // namespace clk