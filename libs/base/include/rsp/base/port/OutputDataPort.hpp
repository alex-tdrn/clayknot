#pragma once

#include "rsp/base/node/AbstractSource.hpp"
#include "rsp/base/port/DataPort.hpp"
#include "rsp/base/port/InputDataPort.hpp"
#include "rsp/base/port/OutputPort.hpp"

namespace rsp
{
template <typename Data>
class InputDataPort;

template <typename Data>
class OutputDataPort final
	: public DataPort
	, public OutputPort<InputDataPort<Data>>
{
private:
	AbstractSource* parent = nullptr;
	Data data;

public:
	OutputDataPort()
	{
		dataTypeHash = std::type_index(typeid(Data)).hash_code();
	}
	OutputDataPort(OutputDataPort const&) = delete;
	OutputDataPort(OutputDataPort&&) = default;
	OutputDataPort& operator=(OutputDataPort const& that) = delete;
	OutputDataPort& operator=(OutputDataPort&&) = default;
	~OutputDataPort() = default;

public:
	Timestamp const& getTimestamp() const final override
	{
		return parent->getTimestamp();
	}

	Data& getData()
	{
		notifyObserverFlags(DataPort::ObserverFlags::onDataRequested);
		return data;
	}

	Data const& getData() const
	{
		notifyObserverFlags(DataPort::ObserverFlags::onDataRequested);
		return data;
	}

	[[nodiscard]] bool update() const final override
	{
		if(!parent->update())
		{
			notifyObserverFlags(DataPort::ObserverFlags::onFailedUpdate);
			return false;
		}
		return true;
	}

	AbstractSource* getParent() const
	{
		return parent;
	}

	void setParent(AbstractSource* parent)
	{
		this->parent = parent;
	}
};

} // namespace rsp