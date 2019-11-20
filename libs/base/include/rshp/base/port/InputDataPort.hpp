#pragma once

#include "rshp/base/port/DataPort.hpp"
#include "rshp/base/port/InputPort.hpp"
#include "rshp/base/port/OutputDataPort.hpp"

#include <typeindex>

namespace rshp::base
{
		
	template<typename Data>
	class OutputDataPort;

	template<typename Data>
	class InputDataPort final : public DataPort, public InputPort<OutputDataPort<Data>>
	{
	public:
		InputDataPort()
		{
			dataTypeHash = std::type_index(typeid(Data)).hash_code();
		}
		InputDataPort(InputDataPort const&) = delete;
		InputDataPort(InputDataPort&&) = default;
		InputDataPort& operator=(InputDataPort const&) = delete;
		InputDataPort& operator=(InputDataPort&&) = default;
		~InputDataPort() = default;

	public:
		Timestamp const& getTimestamp() const final override
		{
			return this->connection->getTimestamp();
		}

		void update() const final override
		{
			if(this->connection)
				this->connection->update();
		}

		Data const& get() const
		{
			requestCount++;
			return this->connection->get();
		}

	};

}