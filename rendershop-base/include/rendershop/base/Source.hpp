#pragma once

#include "rendershop/base/AbstractSource.hpp"
#include "rendershop/base/OutputDataPort.hpp"
#include "rendershop/base/Utility.hpp"
#include "rendershop/base/Timestamp.hpp"

template<typename... O>
struct OutputList
{
	std::tuple<OutputDataPort<O>...> list;
};

template<typename ConcreteSource, typename OutputList>
class Source : public virtual AbstractSource
{
private:
	mutable OutputList outputs;

public:
	Source()
	{
		static_for_index(outputs.list, [&](auto& outputDataPort, int index) {
			AbstractSource::abstractOutputDataPorts.push_back(&outputDataPort);
			outputDataPort.setName(ConcreteSource::OutputPorts::names[index]);
			outputDataPort.setParent(this);
		});
	}

	Source(Source const&) = delete;
	Source(Source&&) = default;
	Source& operator=(Source const&) = delete;
	Source& operator=(Source&&) = default;
	virtual ~Source() = default;

public:
	void run() override
	{
		if(isUpdateQueued())
		{
			update();
			trigger(AbstractPipe::OutputEvents::Updated);
			timestamp.update();
		}
	}

	std::string const& getName() const override
	{
		return ConcreteSource::name;
	}

	template<int outputIndex>
	auto& getOutputDataPort() const
	{
		return std::get<outputIndex>(outputs.list);
	}

	template<int outputIndex>
	auto& getOutputData() const
	{
		return getOutputDataPort<outputIndex>().get();
	}

};