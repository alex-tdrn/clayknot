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
inline std::string DataTypeName<int>::get()
{
	return "Integer";
}

template <>
inline std::string DataTypeName<float>::get()
{
	return "Float";
}

template <>
inline std::string DataTypeName<bool>::get()
{
	return "Boolean";
}

template <>
inline std::string DataTypeName<glm::vec2>::get()
{
	return "Vector2";
}

template <>
inline std::string DataTypeName<glm::vec3>::get()
{
	return "Vector3";
}

template <>
inline std::string DataTypeName<glm::vec4>::get()
{
	return "Vector4";
}
} // namespace rsp
