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
	explicit Node(std::unique_ptr<rsp::Algorithm>&& algorithm);
	Node(Node&&) = delete;
	Node(Node const&) = delete;
	auto operator=(Node const&) -> Node& = delete;
	auto operator=(Node&&) -> Node& = delete;
	~Node() = default;

	void setAlgorithm(std::unique_ptr<rsp::Algorithm>&& algorithm);
	auto getInputPorts() const -> std::vector<rsp::InputPort*> const&;
	auto getOutputPorts() const -> std::vector<rsp::OutputPort*> const&;
	void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {});
	void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {});

private:
	std::unique_ptr<rsp::Algorithm> algorithm;
	std::weak_ptr<rsp::Sentinel> sentinel;

	auto sentinelPresent() const -> bool;
	auto updatePossible() const -> bool;
	auto updateNeeded() const -> bool;
};

inline Node::Node(std::unique_ptr<rsp::Algorithm>&& algorithm)
{
	setAlgorithm(std::move(algorithm));
}

inline void Node::setAlgorithm(std::unique_ptr<rsp::Algorithm>&& algorithm)
{
	this->algorithm = std::move(algorithm);
	for(auto* port : this->algorithm->getInputPorts())
		port->setPushCallback([&](auto sentinel) { this->push(sentinel); });
	for(auto* port : this->algorithm->getOutputPorts())
		port->setPullCallback([&](auto sentinel) { this->pull(sentinel); });
}

inline auto Node::getInputPorts() const -> std::vector<rsp::InputPort*> const&
{
	if(algorithm == nullptr)
		throw std::exception("No algorithm set!");
	return algorithm->getInputPorts();
}

inline auto Node::getOutputPorts() const -> std::vector<rsp::OutputPort*> const&
{
	if(algorithm == nullptr)
		throw std::exception("No algorithm set!");
	return algorithm->getOutputPorts();
}

inline void Node::pull(std::weak_ptr<rsp::Sentinel> const& sentinel)
{
	if(sentinelPresent() || !updatePossible())
		return;

	std::shared_ptr<rsp::Sentinel> sentinelOrigin;
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

inline void Node::push(std::weak_ptr<rsp::Sentinel> const& sentinel)
{
	if(sentinelPresent() || !updatePossible())
		return;

	std::shared_ptr<rsp::Sentinel> sentinelOrigin;
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
	if(algorithm == nullptr)
		return false;
	return std::all_of(getInputPorts().begin(), getInputPorts().end(), std::mem_fn(&rsp::Port::isConnected));
}

inline auto Node::updateNeeded() const -> bool
{
	for(auto* outputPort : getOutputPorts())
		for(auto* inputPort : getInputPorts())
			if(inputPort->getTimestamp() > outputPort->getTimestamp())
				return true;
	return false;
}
} // namespace rsp