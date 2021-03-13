#pragma once

#include "rsp/base/Algorithm.hpp"
#include "rsp/base/Port.hpp"
#include "rsp/base/Sentinel.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace rsp
{
class Node
{
public:
	Node() = default;
	explicit Node(std::unique_ptr<Algorithm>&& algorithm);
	Node(Node const&) = delete;
	Node(Node&&) noexcept = default;
	auto operator=(Node const&) -> Node& = delete;
	auto operator=(Node&&) noexcept -> Node& = default;
	~Node() = default;

	auto getName() const -> std::string;
	void setAlgorithm(std::unique_ptr<Algorithm>&& algorithm);
	auto getInputPorts() const -> std::vector<InputPort*>;
	auto getOutputPorts() const -> std::vector<OutputPort*>;
	void pull(std::weak_ptr<Sentinel> const& sentinel = {});
	void push(std::weak_ptr<Sentinel> const& sentinel = {});

private:
	std::unique_ptr<Algorithm> algorithm;
	std::weak_ptr<Sentinel> sentinel;

	auto sentinelPresent() const -> bool;
	auto updatePossible() const -> bool;
	auto updateNeeded() const -> bool;
};

inline Node::Node(std::unique_ptr<Algorithm>&& algorithm)
{
	setAlgorithm(std::move(algorithm));
}

inline auto Node::getName() const -> std::string
{
	if(algorithm == nullptr)
		return "Empty node";
	return algorithm->getName();
}
inline void Node::setAlgorithm(std::unique_ptr<Algorithm>&& algorithm)
{
	this->algorithm = std::move(algorithm);
	for(auto* port : this->algorithm->getInputPorts())
		port->setPushCallback([&](auto sentinel) {
			this->push(sentinel);
		});
	for(auto* port : this->algorithm->getOutputPorts())
		port->setPullCallback([&](auto sentinel) {
			this->pull(sentinel);
		});
}

inline auto Node::getInputPorts() const -> std::vector<rsp::InputPort*>
{
	if(algorithm == nullptr)
		return {};
	return algorithm->getInputPorts();
}

inline auto Node::getOutputPorts() const -> std::vector<rsp::OutputPort*>
{
	if(algorithm == nullptr)
		return {};
	return algorithm->getOutputPorts();
}

inline void Node::pull(std::weak_ptr<Sentinel> const& sentinel)
{
	if(sentinelPresent() || !updatePossible())
		return;

	std::shared_ptr<Sentinel> sentinelOrigin;
	if(sentinel.expired())
	{
		sentinelOrigin = std::make_shared<Sentinel>();
		this->sentinel = sentinelOrigin;
	}
	else
	{
		this->sentinel = sentinel;
	}

	for(auto* port : getInputPorts())
		port->pull(this->sentinel);

	if(updateNeeded())
		algorithm->update();
}

inline void Node::push(std::weak_ptr<Sentinel> const& sentinel)
{
	if(sentinelPresent() || !updatePossible())
		return;

	std::shared_ptr<Sentinel> sentinelOrigin;
	if(sentinel.expired())
	{
		sentinelOrigin = std::make_shared<Sentinel>();
		this->sentinel = sentinelOrigin;
	}
	else
	{
		this->sentinel = sentinel;
	}

	for(auto* port : getInputPorts())
		port->pull(this->sentinel);

	if(updateNeeded())
		algorithm->update();

	for(auto* port : getOutputPorts())
		port->push(this->sentinel);
}

inline auto Node::sentinelPresent() const -> bool
{
	return !sentinel.expired();
}

inline auto Node::updatePossible() const -> bool
{
	return algorithm != nullptr;
}

inline auto Node::updateNeeded() const -> bool
{
	if(getInputPorts().empty())
		return true;
	if(getOutputPorts().empty())
		return true;

	auto outputs = getOutputPorts();
	auto inputs = getInputPorts();

	return std::any_of(outputs.begin(), outputs.end(), [&](auto const* outputPort) {
		return std::any_of(inputs.begin(), inputs.end(), [&outputPort](auto const* inputPort) {
			return inputPort->getTimestamp() > outputPort->getTimestamp();
		});
	});
}
} // namespace rsp