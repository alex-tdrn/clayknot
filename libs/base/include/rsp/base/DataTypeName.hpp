#pragma once

#include <glm/glm.hpp>
#include <string>
#include <typeinfo>

namespace rsp
{
template <typename DataType>
struct DataTypeName
{
	static std::string get()
	{
		return typeid(DataType).name();
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

template <>
static std::string DataTypeName<glm::vec2>::get()
{
	return "Vector2";
}

template <>
static std::string DataTypeName<glm::vec3>::get()
{
	return "Vector3";
}

template <>
static std::string DataTypeName<glm::vec4>::get()
{
	return "Vector4";
}
} // namespace rsp
