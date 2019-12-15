#pragma once

#include "rsp/base/Timestamp.hpp"
#include "rsp/base/Utility.hpp"
#include "rsp/base/node/AbstractSink.hpp"
#include "rsp/base/port/InputDataPort.hpp"

namespace rsp
{
template <typename... I>
struct InputList
{
	std::tuple<InputDataPort<I>...> list;
};

template <typename ConcreteSink, typename InputList>
class Sink : public virtual AbstractSink
{
protected:
	mutable InputList inputs;

public:
	Sink()
	{
		static_for_index(inputs.list, [&](auto& inputDataPort, int index) {
			AbstractSink::abstractInputDataPorts.push_back(&inputDataPort);
			inputDataPort.setName(ConcreteSink::InputPorts::names[index]);
		});
	}

	Sink(Sink const&) = delete;
	Sink(Sink&&) = default;
	Sink& operator=(Sink const&) = delete;
	Sink& operator=(Sink&&) = default;
	virtual ~Sink() = default;

protected:
	[[nodiscard]] bool updateAllInputs() const override final
	{
		bool success = true;
		static_for(inputs.list, [&](auto const& input) { success = input.update(); });
		return success;
	}

public:
	std::string const& getName() const override
	{
		return ConcreteSink::name;
	}

	template <int inputIndex>
	auto& getInputDataPort() const
	{
		return std::get<inputIndex>(inputs.list);
	}

	template <int inputIndex>
	auto& getInputData() const
	{
		return getInputDataPort<inputIndex>().get();
	}
};

} // namespace rsp