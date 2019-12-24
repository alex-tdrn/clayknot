#pragma once

#include <string>
#include <typeinfo>

namespace rsp
{
template <typename DataType>
struct DataTypeName
{
	static std::string get()
	{
		return "UNKNOWN TYPE";
	}
};

template <>
static std::string DataTypeName<int>::get()
{
	return "Integer";
}

template <>
static std::string DataTypeName<float>::get()
{
	return "Float";
}

template <>
static std::string DataTypeName<bool>::get()
{
	return "Boolean";
}

} // namespace rsp
