#pragma once

#include "rsp/base/Port.hpp"

#include <string>
#include <vector>

namespace rsp
{
class Algorithm
{
public:
	Algorithm(const Algorithm&) = delete;
	Algorithm(Algorithm&&) = delete;
	auto operator=(const Algorithm&) = delete;
	auto operator=(Algorithm&&) = delete;
	virtual ~Algorithm() = default;

	virtual auto getName() const noexcept -> std::string const& = 0;
	virtual void update() = 0;
	auto getInputPorts() const noexcept -> std::vector<InputPort*> const&;
	auto getOutputPorts() const noexcept -> std::vector<OutputPort*> const&;

protected:
	Algorithm() = default;

	void registerPort(InputPort& port);
	void registerPort(OutputPort& port);

private:
	std::vector<InputPort*> inputPorts;
	std::vector<OutputPort*> outputPorts;
};

inline auto Algorithm::getInputPorts() const noexcept -> std::vector<InputPort*> const&
{
	return inputPorts;
}

inline auto Algorithm::getOutputPorts() const noexcept -> std::vector<OutputPort*> const&
{
	return outputPorts;
}

inline void Algorithm::registerPort(InputPort& port)
{
	if(std::find(inputPorts.begin(), inputPorts.end(), &port) != inputPorts.end())
		return;
	inputPorts.emplace_back(&port);
}

inline void Algorithm::registerPort(OutputPort& port)
{
	if(std::find(outputPorts.begin(), outputPorts.end(), &port) != outputPorts.end())
		return;
	outputPorts.emplace_back(&port);
}
} // namespace rsp