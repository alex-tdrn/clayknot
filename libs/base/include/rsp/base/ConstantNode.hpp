#pragma once

#include "rsp/base/Node.hpp"
#include "rsp/base/Port.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace rsp
{
class ConstantNode final : public Node
{
public:
	ConstantNode() = default;
	ConstantNode(ConstantNode const&) = delete;
	ConstantNode(ConstantNode&&) noexcept = delete;
	auto operator=(ConstantNode const&) -> ConstantNode& = delete;
	auto operator=(ConstantNode&&) noexcept -> ConstantNode& = delete;
	~ConstantNode() final = default;

	auto getName() const -> std::string const& final;
	auto getOutputPorts() const -> std::vector<OutputPort*> final;
	void removePort(rsp::OutputPort* port);
	void addPort(std::unique_ptr<rsp::OutputPort>&& port);

private:
	std::vector<std::unique_ptr<rsp::OutputPort>> outputs;
};

inline auto ConstantNode::getName() const -> std::string const&
{
	static std::string const name = "Constant";
	return name;
}

inline auto ConstantNode::getOutputPorts() const -> std::vector<rsp::OutputPort*>
{
	std::vector<rsp::OutputPort*> ret;
	for(auto const& output : outputs)
		ret.push_back(output.get());
	return ret;
}

inline void ConstantNode::removePort(rsp::OutputPort* port)
{
	outputs.erase(std::remove_if(outputs.begin(), outputs.end(),
					  [&](auto& output) {
						  return output.get() == port;
					  }),
		outputs.end());
}

inline void ConstantNode::addPort(std::unique_ptr<rsp::OutputPort>&& port)
{
	outputs.push_back(std::move(port));
}

} // namespace rsp