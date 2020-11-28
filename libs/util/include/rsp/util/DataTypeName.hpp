#pragma once

#include <glm/glm.hpp>
#include <string>
#include <typeinfo>

namespace rsp
{
template <typename DataType>
struct DataTypeName
{
	static auto get() -> std::string
	{
		return typeid(DataType).name();
	}
};

template <>
inline auto DataTypeName<int>::get() -> std::string
{
	return "Integer";
}

template <>
inline auto DataTypeName<float>::get() -> std::string
{
	return "Float";
}

template <>
inline auto DataTypeName<bool>::get() -> std::string
{
	return "Boolean";
}

template <>
inline auto DataTypeName<glm::vec2>::get() -> std::string
{
	return "Vector2";
}

template <>
inline auto DataTypeName<glm::vec3>::get() -> std::string
{
	return "Vector3";
}

template <>
inline auto DataTypeName<glm::vec4>::get() -> std::string
{
	return "Vector4";
}
} // namespace rsp
