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

	Data& get()
	{
		return data;
	}

	Data const& get() const
	{
		return data;
	}

	void update() const final override
	{
		parent->run();
	}

	void setParent(AbstractSource* parent)
	{
		this->parent = parent;
	}
};

} // namespace rsp