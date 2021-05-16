#pragma once

#include "rsp/util/data_type_name.hpp"

#include <cassert>
#include <glm/glm.hpp>

namespace rsp
{
template <typename T>
class bounded
{
public:
	bounded() = default;
	explicit bounded(T val, T min, T max);
	bounded(bounded const&) = default;
	bounded(bounded&&) noexcept = default;
	auto operator=(bounded const& that) -> bounded& = default;
	auto operator=(bounded&& that) noexcept -> bounded& = default;
	auto operator=(T const& that) -> bounded&;
	auto operator=(T&& that) -> bounded&;
	~bounded() = default;

	auto valid() -> bool;
	void set_min(T min);
	void set_max(T max);
	auto get_min() const -> T;
	auto get_max() const -> T;
	auto get_val() const -> T;
	auto data() -> T*;
	explicit operator T() const;
	auto operator+=(T const& that) -> bounded&;
	auto operator-=(T const& that) -> bounded&;
	auto operator*=(T const& that) -> bounded&;
	auto operator/=(T const& that) -> bounded&;

private:
	T val;
	T min;
	T max;
};

template <typename T>
bounded<T>::bounded(T val, T min, T max) : val(val), min(min), max(max)
{
	assert(valid());
}

template <typename T>
auto bounded<T>::operator=(T const& that) -> bounded&
{
	val = that;
	assert(valid());
}

template <typename T>
auto bounded<T>::operator=(T&& that) -> bounded&
{
	val = std::move(that);
	assert(valid());
}

template <typename T>
auto bounded<T>::valid() -> bool
{
	return min <= max && val >= min && val <= max;
}

template <>
inline auto bounded<glm::vec2>::valid() -> bool
{
	for(int i = 0; i < 2; i++)
	{
		if(min[i] > max[i] || val[i] < min[i] || val[i] > max[i])
			return false;
	}
	return true;
}

template <>
inline auto bounded<glm::vec3>::valid() -> bool
{
	for(int i = 0; i < 3; i++)
	{
		if(min[i] > max[i] || val[i] < min[i] || val[i] > max[i])
			return false;
	}
	return true;
}

template <>
inline auto bounded<glm::vec4>::valid() -> bool
{
	for(int i = 0; i < 4; i++)
	{
		if(min[i] > max[i] || val[i] < min[i] || val[i] > max[i])
			return false;
	}
	return true;
}

template <typename T>
void bounded<T>::set_min(T min)
{
	this->min = min;
	assert(valid());
}

template <typename T>
void bounded<T>::set_max(T max)
{
	this->max = max;
	assert(valid());
}

template <typename T>
auto bounded<T>::get_min() const -> T
{
	return min;
}

template <typename T>
auto bounded<T>::get_max() const -> T
{
	return max;
}

template <typename T>
auto bounded<T>::get_val() const -> T
{
	return val;
}

template <typename T>
auto bounded<T>::data() -> T*
{
	return &val;
}

template <typename T>
bounded<T>::operator T() const
{
	return val;
}

template <typename T>
auto bounded<T>::operator+=(T const& that) -> bounded&
{
	this->value += that;
	assert(valid());
	return *this;
}

template <typename T>
auto bounded<T>::operator-=(T const& that) -> bounded&
{
	this->value -= that;
	assert(valid());
	return *this;
}

template <typename T>
auto bounded<T>::operator*=(T const& that) -> bounded&
{
	this->value *= that;
	assert(valid());
	return *this;
}

template <typename T>
auto bounded<T>::operator/=(T const& that) -> bounded&
{
	this->value /= that;
	assert(valid());
	return *this;
}

template <typename DataType>
struct data_type_name<bounded<DataType>>
{
	static auto get() -> std::string
	{
		return "bounded " + data_type_name<DataType>::get();
	}
};

} // namespace rsp