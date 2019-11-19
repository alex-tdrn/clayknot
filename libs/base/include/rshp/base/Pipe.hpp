#pragma once

#include "rshp/base/Source.hpp"
#include "rshp/base/Sink.hpp"

namespace rshp::base
{
		
	template<typename ConcretePipe, typename InputList, typename OutputList>
	class Pipe : virtual public Sink<ConcretePipe, InputList>, virtual public Source<ConcretePipe, OutputList>
	{
	public:
		struct InputEvents
		{
			enum
			{
			};
		};

		struct OutputEvents
		{
			enum
			{
			};
		};

	public:
		Pipe() = default;
		Pipe(Pipe const&) = delete;
		Pipe(Pipe&&) = default;
		Pipe& operator=(Pipe const&) = delete;
		Pipe& operator=(Pipe&&) = default;
		virtual ~Pipe() = default;

	private:
		void registerInputEvents() override
		{
			AbstractSink::registerInputEvents();
			AbstractSource::registerInputEvents();
		}

		void registerOutputEvents() override
		{
			AbstractSink::registerOutputEvents();
			AbstractSource::registerOutputEvents();
		}

	public:
		std::string const& getName() const override final
		{
			return ConcretePipe::name;
		}

		void run() override
		{
			if(!this->allInputsConnected())
				return;

			this->updateAllInputs();

			if(!this->isUpdateQueued())
			{
				bool outputsOutdated = false;
				static_for(this->inputs.list, [&](auto const& input) {
					if(input.getTimestamp().isNewerThan(this->timestamp))
						outputsOutdated = true;
				});

				if(!outputsOutdated)
					return;
			}

			this->update();
			this->timestamp.update();
			this->trigger(AbstractPipe::OutputEvents::Updated);
		}

	};
	
}