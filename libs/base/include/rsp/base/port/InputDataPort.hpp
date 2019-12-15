#pragma once

#include "rsp/base/port/DataPort.hpp"
#include "rsp/base/port/InputPort.hpp"
#include "rsp/base/port/OutputDataPort.hpp"

#include <typeindex>

namespace rsp
{
template <typename Data>
class OutputDataPort;

template <typename Data>
class InputDataPort final
	: public DataPort
	, public InputPort<OutputDataPort<Data>>
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

	[[nodiscard]] bool update() const final override
	{
		bool success = false;
		if(this->connection)
			success = this->connection->update();
		if(!success)
			notifyObserverFlags(DataPort::ObserverFlags::onFailedUpdate);
		return success;
	}

	Data const& get() const
	{
		notifyObserverFlags(DataPort::ObserverFlags::onDataRequested);
		return this->connection->get();
	}
};

} // namespace rsp