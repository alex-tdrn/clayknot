#pragma once

#include "rsp/base/DataTypeName.hpp"
#include "rsp/base/node/Source.hpp"

#include <array>

namespace rsp
{
template <typename DataType>
class FixedSource : public rsp::Source<FixedSource<DataType>, rsp::OutputList<DataType>>
{
public:
	struct OutputPorts
	{
		static inline std::array names = {"Data"};
		enum
		{
			Data
		};
	};

public:
	static inline std::string const name =
		registerNode<FixedSource<DataType>>("Fixed Source <" + rsp::DataTypeName<DataType>::get() + ">");

protected:
	void run() override
	{
	}

public:
	DataType getFixedData() const
	{
		return getOutputData<OutputPorts::Data>();
	}

	void setFixedData(DataType data)
	{
		getOutputData<OutputPorts::Data>() = data;
		outputsUpdated();
	}
};

} // namespace rsp