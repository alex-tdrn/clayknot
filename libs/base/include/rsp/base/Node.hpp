#pragma once

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
// TODO move update() to separate Algorithm class contained in the node.
class Node
{
public:
	Node(Node&&) = delete;
	Node(Node const&) = delete;
	auto operator=(Node const&) -> Node& = delete;
	auto operator=(Node&&) -> Node& = delete;
	virtual ~Node() = default;

	auto getInputPorts() const noexcept -> std::vector<rsp::Port*> const&;
	auto getOutputPorts() const noexcept -> std::vector<rsp::Port*> const&;
	auto getInputPort(std::size_t index) const noexcept -> rsp::Port&;
	auto getOutputPort(std::size_t index) const noexcept -> rsp::Port&;
	void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {});
	void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {});
	virtual auto getName() const -> std::string const& = 0;

protected:
	Node() = default;

	template <typename T>
	void registerPort(rsp::InputPortOf<T>& port);
	template <typename T>
	void registerPort(rsp::OutputPortOf<T>& port);
	virtual void update() = 0;

private:
	std::vector<rsp::Port*> inputPorts;
	std::vector<rsp::Port*> outputPorts;
	std::weak_ptr<rsp::Sentinel> sentinel;

	auto sentinelPresent() const -> bool;
	auto updatePossible() const -> bool;
	auto updateNeeded() const -> bool;
};

template <typename T>
void Node::registerPort(rsp::InputPortOf<T>& port)
{
	if(std::find(inputPorts.begin(), inputPorts.end(), &port) != inputPorts.end())
		return;
	port.setPushCallback([&](auto sentinel) { this->push(sentinel); });
	inputPorts.push_back(&port);
}

template <typename T>
void Node::registerPort(rsp::OutputPortOf<T>& port)
{
	if(std::find(outputPorts.begin(), outputPorts.end(), &port) != outputPorts.end())
		return;
	port.setPullCallback([&](auto sentinel) { this->pull(sentinel); });
	outputPorts.push_back(&port);
}

inline auto Node::getInputPorts() const noexcept -> std::vector<rsp::Port*> const&
{
	return inputPorts;
}

inline auto Node::getOutputPorts() const noexcept -> std::vector<rsp::Port*> const&
{
	return outputPorts;
}

inline auto Node::getInputPort(std::size_t index) const noexcept -> rsp::Port&
{
	assert(index < inputPorts.size());
	return *inputPorts[index];
}

inline auto Node::getOutputPort(std::size_t index) const noexcept -> rsp::Port&
{
	assert(index < outputPorts.size());
	return *outputPorts[index];
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

	for(auto* port : inputPorts)
		port->pull(this->sentinel);

	if(updateNeeded())
		update();
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

	for(auto* port : inputPorts)
		port->pull(this->sentinel);

	if(updateNeeded())
		update();

	for(auto* port : outputPorts)
		port->push(this->sentinel);
}

inline auto Node::sentinelPresent() const -> bool
{
	return !sentinel.expired();
}

inline auto Node::updatePossible() const -> bool
{
	return std::all_of(inputPorts.begin(), inputPorts.end(), std::mem_fn(&rsp::Port::isConnected));
}

inline auto Node::updateNeeded() const -> bool
{
	for(auto* outputPort : outputPorts)
		for(auto* inputPort : inputPorts)
			if(inputPort->getTimestamp() > outputPort->getTimestamp())
				return true;
	return false;
}
} // namespace rsp