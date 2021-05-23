#pragma once

#include "clk/util/data_type_name.hpp"

#include <cassert>
#include <glm/glm.hpp>

namespace clk
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
	T _val;
	T _min;
	T _max;
};

template <typename T>
bounded<T>::bounded(T val, T min, T max) : _val(val), _min(min), _max(max)
{
	assert(valid());
}

template <typename T>
auto bounded<T>::operator=(T const& that) -> bounded&
{
	_val = that;
	assert(valid());
}

template <typename T>
auto bounded<T>::operator=(T&& that) -> bounded&
{
	_val = std::move(that);
	assert(valid());
}

template <typename T>
auto bounded<T>::valid() -> bool
{
	return _min <= _max && _val >= _min && _val <= _max;
}

template <>
inline auto bounded<glm::vec2>::valid() -> bool
{
	for(int i = 0; i < 2; i++)
	{
		if(_min[i] > _max[i] || _val[i] < _min[i] || _val[i] > _max[i])
			return false;
	}
	return true;
}

template <>
inline auto bounded<glm::vec3>::valid() -> bool
{
	for(int i = 0; i < 3; i++)
	{
		if(_min[i] > _max[i] || _val[i] < _min[i] || _val[i] > _max[i])
			return false;
	}
	return true;
}

template <>
inline auto bounded<glm::vec4>::valid() -> bool
{
	for(int i = 0; i < 4; i++)
	{
		if(_min[i] > _max[i] || _val[i] < _min[i] || _val[i] > _max[i])
			return false;
	}
	return true;
}

template <typename T>
void bounded<T>::set_min(T min)
{
	_min = min;
	assert(valid());
}

template <typename T>
void bounded<T>::set_max(T max)
{
	_max = max;
	assert(valid());
}

template <typename T>
auto bounded<T>::get_min() const -> T
{
	return _min;
}

template <typename T>
auto bounded<T>::get_max() const -> T
{
	return _max;
}

template <typename T>
auto bounded<T>::get_val() const -> T
{
	return _val;
}

template <typename T>
auto bounded<T>::data() -> T*
{
	return &_val;
}

template <typename T>
bounded<T>::operator T() const
{
	return _val;
}

template <typename T>
auto bounded<T>::operator+=(T const& that) -> bounded&
{
	_val += that;
	assert(valid());
	return *this;
}

template <typename T>
auto bounded<T>::operator-=(T const& that) -> bounded&
{
	_val -= that;
	assert(valid());
	return *this;
}

template <typename T>
auto bounded<T>::operator*=(T const& that) -> bounded&
{
	_val *= that;
	assert(valid());
	return *this;
}

template <typename T>
auto bounded<T>::operator/=(T const& that) -> bounded&
{
	_val /= that;
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

} // namespace clk