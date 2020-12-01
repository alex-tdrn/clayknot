#pragma once

#include "rsp/util/DataTypeName.hpp"

#include <cassert>
#include <glm/glm.hpp>

namespace rsp
{
template <typename T>
class Bounded
{
private:
	T val;
	T min;
	T max;

public:
	Bounded() = default;
	explicit Bounded(T val, T min, T max) : val(val), min(min), max(max)
	{
		assert(valid());
	}
	Bounded(Bounded const&) = default;
	Bounded(Bounded&&) noexcept = default;
	auto operator=(Bounded const& that) -> Bounded& = default;
	auto operator=(Bounded&& that) noexcept -> Bounded& = default;
	auto operator=(T const& that) -> Bounded&
	{
		val = that;
		assert(valid());
	}
	auto operator=(T&& that) -> Bounded&
	{
		val = std::move(that);
		assert(valid());
	}
	~Bounded() = default;

	auto valid() -> bool
	{
		return min <= max && val >= min && val <= max;
	}

	void setMin(T min)
	{
		this->min = min;
		assert(valid());
	}

	void setMax(T max)
	{
		this->max = max;
		assert(valid());
	}

	auto getMin() const -> T
	{
		return min;
	}

	auto getMax() const -> T
	{
		return max;
	}

	auto getVal() const -> T
	{
		return val;
	}

	auto data() -> T*
	{
		return &val;
	}

	explicit operator T() const
	{
		return val;
	}

	auto operator+=(T const& that) -> Bounded&
	{
		this->value += that;
		assert(valid());
		return *this;
	}

	auto operator-=(T const& that) -> Bounded&
	{
		this->value -= that;
		assert(valid());
		return *this;
	}

	auto operator*=(T const& that) -> Bounded&
	{
		this->value *= that;
		assert(valid());
		return *this;
	}

	auto operator/=(T const& that) -> Bounded&
	{
		this->value /= that;
		assert(valid());
		return *this;
	}
};
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

template <typename DataType>
struct DataTypeName<Bounded<DataType>>
{
	static auto get() -> std::string
	{
		return "Bounded " + DataTypeName<DataType>::get();
	}
};

} // namespace rsp