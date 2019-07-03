#pragma once

#include "rendershop/base/AbstractSink.hpp"
#include "rendershop/base/InputDataPort.hpp"
#include "rendershop/base/Utility.hpp"
#include "rendershop/base/Timestamp.hpp"

template<typename... I>
struct InputList
{
	std::tuple<InputDataPort<I>...> list;
};

template<typename ConcreteSink, typename InputList>
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
	void updateAllInputs() const override final
	{
		static_for(inputs.list, [](auto const& input) {
			input.update();
		});
	}

public:
	std::string const& getName() const override
	{
		return ConcreteSink::name;
	}

	template<int inputIndex>
	auto& getInputDataPort() const
	{
		return std::get<inputIndex>(inputs.list);
	}

	template<int inputIndex>
	auto& getInputData() const
	{
		return getInputDataPort<inputIndex>().get();
	}

};