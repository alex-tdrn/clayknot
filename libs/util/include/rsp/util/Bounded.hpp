#pragma once

#include "rsp/util/DataTypeName.hpp"

#include <cassert>
#include <glm/glm.hpp>

namespace rsp
{
template <typename T>
class Bounded
{
public:
	Bounded() = default;
	explicit Bounded(T val, T min, T max);
	Bounded(Bounded const&) = default;
	Bounded(Bounded&&) noexcept = default;
	auto operator=(Bounded const& that) -> Bounded& = default;
	auto operator=(Bounded&& that) noexcept -> Bounded& = default;
	auto operator=(T const& that) -> Bounded&;
	auto operator=(T&& that) -> Bounded&;
	~Bounded() = default;

	auto valid() -> bool;
	void setMin(T min);
	void setMax(T max);
	auto getMin() const -> T;
	auto getMax() const -> T;
	auto getVal() const -> T;
	auto data() -> T*;
	explicit operator T() const;
	auto operator+=(T const& that) -> Bounded&;
	auto operator-=(T const& that) -> Bounded&;
	auto operator*=(T const& that) -> Bounded&;
	auto operator/=(T const& that) -> Bounded&;

private:
	T val;
	T min;
	T max;
};

template <typename T>
Bounded<T>::Bounded(T val, T min, T max) : val(val), min(min), max(max)
{
	assert(valid());
}

template <typename T>
auto Bounded<T>::operator=(T const& that) -> Bounded&
{
	val = that;
	assert(valid());
}

template <typename T>
auto Bounded<T>::operator=(T&& that) -> Bounded&
{
	val = std::move(that);
	assert(valid());
}

template <typename T>
auto Bounded<T>::valid() -> bool
{
	return min <= max && val >= min && val <= max;
}

template <>
inline auto Bounded<glm::vec2>::valid() -> bool
{
	for(int i = 0; i < 2; i++)
	{
		if(min[i] > max[i] || val[i] < min[i] || val[i] > max[i])
			return false;
	}
	return true;
}

template <>
inline auto Bounded<glm::vec3>::valid() -> bool
{
	for(int i = 0; i < 3; i++)
	{
		if(min[i] > max[i] || val[i] < min[i] || val[i] > max[i])
			return false;
	}
	return true;
}

template <>
inline auto Bounded<glm::vec4>::valid() -> bool
{
	for(int i = 0; i < 4; i++)
	{
		if(min[i] > max[i] || val[i] < min[i] || val[i] > max[i])
			return false;
	}
	return true;
}

template <typename T>
void Bounded<T>::setMin(T min)
{
	this->min = min;
	assert(valid());
}

template <typename T>
void Bounded<T>::setMax(T max)
{
	this->max = max;
	assert(valid());
}

template <typename T>
auto Bounded<T>::getMin() const -> T
{
	return min;
}

template <typename T>
auto Bounded<T>::getMax() const -> T
{
	return max;
}

template <typename T>
auto Bounded<T>::getVal() const -> T
{
	return val;
}

template <typename T>
auto Bounded<T>::data() -> T*
{
	return &val;
}

template <typename T>
Bounded<T>::operator T() const
{
	return val;
}

template <typename T>
auto Bounded<T>::operator+=(T const& that) -> Bounded&
{
	this->value += that;
	assert(valid());
	return *this;
}

template <typename T>
auto Bounded<T>::operator-=(T const& that) -> Bounded&
{
	this->value -= that;
	assert(valid());
	return *this;
}

template <typename T>
auto Bounded<T>::operator*=(T const& that) -> Bounded&
{
	this->value *= that;
	assert(valid());
	return *this;
}

template <typename T>
auto Bounded<T>::operator/=(T const& that) -> Bounded&
{
	this->value /= that;
	assert(valid());
	return *this;
}

template <typename DataType>
struct DataTypeName<Bounded<DataType>>
{
	static auto get() -> std::string
	{
		return "Bounded " + DataTypeName<DataType>::get();
	}
};

} // namespace rsp