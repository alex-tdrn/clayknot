#pragma once

#include <glm/glm.hpp>
#include <string>
#include <typeinfo>

namespace rsp
{
template <typename DataType>
struct data_type_name
{
	static auto get() -> std::string
	{
		return typeid(DataType).name();
	}
};

template <>
inline auto data_type_name<int>::get() -> std::string
{
	return "Integer";
}

template <>
inline auto data_type_name<float>::get() -> std::string
{
	return "Float";
}

template <>
inline auto data_type_name<bool>::get() -> std::string
{
	return "Boolean";
}

template <>
inline auto data_type_name<glm::vec2>::get() -> std::string
{
	return "Vector2";
}

template <>
inline auto data_type_name<glm::vec3>::get() -> std::string
{
	return "Vector3";
}

template <>
inline auto data_type_name<glm::vec4>::get() -> std::string
{
	return "Vector4";
}
} // namespace rsp
