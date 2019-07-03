#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>

#include "EventPipe.hpp"

class AbstractPipe : public virtual EventPipe
{
public:
	struct InputEvents
	{
		enum
		{
			SourceEvents = 8,
			SinkEvents = 16,
			PipeEvents = 24,
			UserEvents = 32
		};
	};

	struct OutputEvents
	{
		enum
		{
			Updated,
			SourceEvents = 8,
			SinkEvents = 16,
			PipeEvents = 24,
			UserEvents = 32
		};
	};

public:
	AbstractPipe() = default;
	AbstractPipe(AbstractPipe const&) = delete;
	AbstractPipe(AbstractPipe&&) = default;
	AbstractPipe& operator=(AbstractPipe const&) = delete;
	AbstractPipe& operator=(AbstractPipe&&) = default;
	virtual ~AbstractPipe() = default;

private:
	static std::unordered_map<std::string, std::unique_ptr<AbstractPipe>(*)()>& pipeMap()
	{
		static std::unordered_map<std::string, std::unique_ptr<AbstractPipe>(*)()> allPipes;
		return allPipes;
	}

protected:
	virtual void update() = 0;
	
	template <typename ConcretePipe>
	static std::string registerPipe(std::string name)
	{
		pipeMap()[name] = []() {
			std::unique_ptr<AbstractPipe> ptr = std::make_unique<ConcretePipe>();
			return ptr;
		};
		return name;
	}

public:
	virtual void run() = 0;

	void registerInputEvents() override
	{
	}

	void registerOutputEvents() override
	{
		registerOutputEvent(OutputEvents::Updated, "Updated");
	}

	static std::unordered_map<std::string, std::unique_ptr<AbstractPipe>(*)()> const& getPipeMap()
	{
		return pipeMap();
	}

	static std::unique_ptr<AbstractPipe> createPipe(std::string const name)
	{
		assert(pipeMap().find(name) != pipeMap().end());

		return pipeMap()[name]();
	}

	virtual std::string const& getName() const = 0;

};
