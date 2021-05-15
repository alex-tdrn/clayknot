#pragma once

#include "rsp/base/Algorithm.hpp"
#include "rsp/base/Node.hpp"
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
class AlgorithmNode final : public Node
{
public:
	AlgorithmNode() = default;
	explicit AlgorithmNode(std::unique_ptr<Algorithm>&& algorithm);
	AlgorithmNode(AlgorithmNode const&) = delete;
	AlgorithmNode(AlgorithmNode&&) noexcept = delete;
	auto operator=(AlgorithmNode const&) -> AlgorithmNode& = delete;
	auto operator=(AlgorithmNode&&) noexcept -> AlgorithmNode& = delete;
	~AlgorithmNode() final = default;

	auto getName() const -> std::string const& final;
	void setAlgorithm(std::unique_ptr<Algorithm>&& algorithm);
	auto getInputPorts() const -> std::vector<InputPort*> final;
	auto getOutputPorts() const -> std::vector<OutputPort*> final;
	void pull(std::weak_ptr<Sentinel> const& sentinel = {}) final;
	void push(std::weak_ptr<Sentinel> const& sentinel = {}) final;

private:
	std::unique_ptr<Algorithm> algorithm;
	std::weak_ptr<Sentinel> sentinel;

	auto sentinelPresent() const -> bool;
	auto updatePossible() const -> bool;
	auto updateNeeded() const -> bool;
};

inline AlgorithmNode::AlgorithmNode(std::unique_ptr<Algorithm>&& algorithm)
{
	setAlgorithm(std::move(algorithm));
}

inline auto AlgorithmNode::getName() const -> std::string const&
{
	static std::string const emptyName = "Empty algorithm node";
	if(algorithm == nullptr)
		return emptyName;
	return algorithm->getName();
}

inline void AlgorithmNode::setAlgorithm(std::unique_ptr<Algorithm>&& algorithm)
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

inline auto AlgorithmNode::getInputPorts() const -> std::vector<rsp::InputPort*>
{
	if(algorithm == nullptr)
		return {};
	return algorithm->getInputPorts();
}

inline auto AlgorithmNode::getOutputPorts() const -> std::vector<rsp::OutputPort*>
{
	if(algorithm == nullptr)
		return {};
	return algorithm->getOutputPorts();
}

inline void AlgorithmNode::pull(std::weak_ptr<Sentinel> const& sentinel)
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

	Node::pull(sentinel);

	if(updateNeeded())
		algorithm->update();
}

inline void AlgorithmNode::push(std::weak_ptr<Sentinel> const& sentinel)
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

	Node::pull(sentinel);

	if(updateNeeded())
		algorithm->update();

	Node::push(sentinel);
}

inline auto AlgorithmNode::sentinelPresent() const -> bool
{
	return !sentinel.expired();
}

inline auto AlgorithmNode::updatePossible() const -> bool
{
	return algorithm != nullptr;
}

inline auto AlgorithmNode::updateNeeded() const -> bool
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