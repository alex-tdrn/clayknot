#pragma once

#include "rsp/base/Port.hpp"

#include <memory>
#include <string>
#include <vector>

namespace rsp
{
class Sentinel;
class InputPort;
class OutputPort;

class Node
{
public:
	Node() = default;
	Node(Node const&) = delete;
	Node(Node&&) noexcept = delete;
	auto operator=(Node const&) -> Node& = delete;
	auto operator=(Node&&) noexcept -> Node& = delete;
	virtual ~Node() = default;

	virtual auto getName() const -> std::string const& = 0;
	virtual auto getInputPorts() const -> std::vector<InputPort*>;
	virtual auto getOutputPorts() const -> std::vector<OutputPort*>;
	virtual void pull(std::weak_ptr<Sentinel> const& sentinel = {});
	virtual void push(std::weak_ptr<Sentinel> const& sentinel = {});
};

inline auto Node::getInputPorts() const -> std::vector<InputPort*>
{
	return {};
}

inline auto Node::getOutputPorts() const -> std::vector<OutputPort*>
{
	return {};
}

inline void Node::pull(std::weak_ptr<Sentinel> const& sentinel)
{
	for(auto* port : getInputPorts())
		port->pull(sentinel);
}

inline void Node::push(std::weak_ptr<Sentinel> const& sentinel)
{
	for(auto* port : getOutputPorts())
		port->push(sentinel);
}

} // namespace rsp