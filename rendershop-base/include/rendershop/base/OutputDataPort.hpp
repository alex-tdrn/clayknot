#pragma once

#include "rendershop/base/AbstractDataPort.hpp"
#include "rendershop/base/OutputPort.hpp"
#include "rendershop/base/InputDataPort.hpp"
#include "rendershop/base/AbstractSource.hpp"

template<typename Data>
class InputDataPort;

template<typename Data>
class OutputDataPort final : public AbstractDataPort, public OutputPort<InputDataPort<Data>>
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