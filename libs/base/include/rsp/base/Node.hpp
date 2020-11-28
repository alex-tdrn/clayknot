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
class Node
{
private:
	std::vector<rsp::Port*> inputPorts;
	std::vector<rsp::Port*> outputPorts;
	std::weak_ptr<rsp::Sentinel> sentinel;

protected:
	Node() = default;

public:
	Node(Node&&) = delete;
	Node(Node const&) = delete;
	auto operator=(Node const&) -> Node& = delete;
	auto operator=(Node&&) -> Node& = delete;
	virtual ~Node() = default;

private:
	auto sentinelPresent() const -> bool
	{
		return !sentinel.expired();
	}

	auto updatePossible() const -> bool
	{
		return std::all_of(inputPorts.begin(), inputPorts.end(), std::mem_fn(&rsp::Port::isConnected));
	}

	auto updateNeeded() const -> bool
	{
		for(auto* outputPort : outputPorts)
			for(auto* inputPort : inputPorts)
				if(inputPort->getTimestamp() > outputPort->getTimestamp())
					return true;
		return false;
	}

protected:
	template <typename T>
	void registerPort(rsp::InputPort<T>& port)
	{
		for(auto* inputPort : inputPorts)
			if(inputPort == &port)
				return;
		port.setPushCallback([&](auto sentinel) { this->push(sentinel); });
		inputPorts.push_back(&port);
	}

	template <typename T>
	void registerPort(rsp::OutputPort<T>& port)
	{
		for(auto* outputPort : outputPorts)
			if(outputPort == &port)
				return;
		port.setPullCallback([&](auto sentinel) { this->pull(sentinel); });
		outputPorts.push_back(&port);
	}

	virtual void update() = 0;

public:
	virtual auto getName() const -> std::string const& = 0;

	auto getInputPorts() const noexcept -> std::vector<rsp::Port*> const&
	{
		return inputPorts;
	}

	auto getOutputPorts() const noexcept -> std::vector<rsp::Port*> const&
	{
		return outputPorts;
	}

	auto getInputPort(std::size_t index) const noexcept -> rsp::Port&
	{
		assert(index < inputPorts.size());
		return *inputPorts[index];
	}

	auto getOutputPort(std::size_t index) const noexcept -> rsp::Port&
	{
		assert(index < outputPorts.size());
		return *outputPorts[index];
	}

	void pull(std::weak_ptr<rsp::Sentinel> const& sentinel = {})
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

	void push(std::weak_ptr<rsp::Sentinel> const& sentinel = {})
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
};
} // namespace rsp