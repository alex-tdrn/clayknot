#pragma once

#include <glm/glm.hpp>

#include <cassert>

namespace rsp
{
template <typename T>
class Bounded
{
private:
	T min;
	T max;
	T val;

public:
	Bounded() = default;
	explicit Bounded(T val, T min, T max) : val(val), min(min), max(max)
	{
		assert(valid());
	}
	Bounded(Bounded const&) = default;
	Bounded(Bounded&&) = default;
	Bounded& operator=(Bounded const& that) = default;
	Bounded& operator=(Bounded&& that) = default;
	Bounded& operator=(T const& that)
	{
		val = that;
		assert(valid());
	}
	Bounded& operator=(T&& that)
	{
		val = std::move(that);
		assert(valid());
	}
	~Bounded() = default;

	bool valid()
	{
		if(min > max || val < min || val > max)
			return false;
		return true;
	}

	void setMin(T min)
	{
		this->min = min;
		assert(valid);
	}

	void setMax(T max)
	{
		this->max = max;
		assert(valid);
	}

	T getMin() const
	{
		return min;
	}

	T getMax() const
	{
		return max;
	}

	T getVal() const
	{
		return val;
	}

	T* data()
	{
		return &val;
	}

	operator T() const
	{
		return val;
	}

	Bounded& operator+=(T const& that)
	{
		this->value += that;
		assert(valid());
		return *this;
	}

	Bounded& operator-=(T const& that)
	{
		this->value -= that;
		assert(valid());
		return *this;
	}

	Bounded& operator*=(T const& that)
	{
		this->value *= that;
		assert(valid());
		return *this;
	}

	Bounded& operator/=(T const& that)
	{
		this->value /= that;
		assert(valid());
		return *this;
	}
};
template <>
inline bool Bounded<glm::vec2>::valid()
{
	for(int i = 0; i < 2; i++)
	{
		if(min[i] > max[i] || val[i] < min[i] || val[i] > max[i])
			return false;
	}
	return true;
}

template <>
inline bool Bounded<glm::vec3>::valid()
{
	for(int i = 0; i < 3; i++)
	{
		if(min[i] > max[i] || val[i] < min[i] || val[i] > max[i])
			return false;
	}
	return true;
}

template <>
inline bool Bounded<glm::vec4>::valid()
{
	for(int i = 0; i < 4; i++)
	{
		if(min[i] > max[i] || val[i] < min[i] || val[i] > max[i])
			return false;
	}
	return true;
}

} // namespace rsp