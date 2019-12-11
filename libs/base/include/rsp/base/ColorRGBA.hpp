#pragma once

#include "rsp/base/ColorRGB.hpp"

#include <cassert>

namespace rsp
{
class ColorRGBA
{
private:
	ColorRGB rgbValue;
	float alphaValue = 1.0f;

public:
	ColorRGBA() = default;
	explicit ColorRGBA(glm::vec4 v) : rgbValue(v.r, v.g, v.b), alphaValue(v.a)
	{
	}
	explicit ColorRGBA(ColorRGB v) : rgbValue(v)
	{
	}
	explicit ColorRGBA(float v) : rgbValue(v), alphaValue(v)
	{
	}
	explicit ColorRGBA(float r, float g, float b, float a) : rgbValue(r, g, b), alphaValue(a)
	{
	}
	explicit ColorRGBA(int r, int g, int b, int a)
		: rgbValue(r / 255.0f, g / 255.0f, b / 255.0f), alphaValue(a / 255.0f)
	{
	}
	ColorRGBA(ColorRGBA const&) = default;
	ColorRGBA(ColorRGBA&&) = default;
	ColorRGBA& operator=(ColorRGBA const& that) = default;
	ColorRGBA& operator=(ColorRGBA&& that) = default;
	~ColorRGBA() = default;

public:
	bool valid() const
	{
		if(!rgbValue.valid())
			return false;
		if(alphaValue < 0.0f || alphaValue > 1.0f)
			return false;
		return true;
	}

	template <typename T>
	ColorRGBA& operator+=(T const& that)
	{
		this->rgbValue += that;
		this->alphaValue += that;
		assert(valid());
		return *this;
	}

	ColorRGBA& operator+=(ColorRGBA const& that)
	{
		this->rgbValue += that.rgbValue;
		this->alphaValue += that.alphaValue;
		assert(valid());
		return *this;
	}

	template <typename T>
	ColorRGBA& operator-=(T const& that)
	{
		this->rgbValue -= that;
		this->alphaValue -= that;
		assert(valid());
		return *this;
	}

	ColorRGBA& operator-=(ColorRGBA const& that)
	{
		this->rgbValue -= that.rgbValue;
		this->alphaValue -= that.alphaValue;
		assert(valid());
		return *this;
	}

	template <typename T>
	ColorRGBA& operator*=(T const& that)
	{
		this->rgbValue *= that;
		this->alphaValue *= that;
		assert(valid());
		return *this;
	}

	ColorRGBA& operator*=(ColorRGBA const& that)
	{
		this->rgbValue *= that.rgbValue;
		this->alphaValue *= that.alphaValue;
		assert(valid());
		return *this;
	}

	template <typename T>
	ColorRGBA& operator/=(T const& that)
	{
		this->rgbValue /= that;
		this->alphaValue /= that;
		assert(valid());
		return *this;
	}

	ColorRGBA& operator/=(ColorRGBA const& that)
	{
		this->rgbValue /= that.rgbValue;
		this->alphaValue /= that.alphaValue;
		assert(valid());
		return *this;
	}

	float* data()
	{
		return rgbValue.data();
	}

	float r() const
	{
		return rgbValue.r();
	}

	float g() const
	{
		return rgbValue.g();
	}

	float b() const
	{
		return rgbValue.b();
	}

	float a() const
	{
		return alphaValue;
	}

	float operator[](std::size_t i) const
	{
		assert(i <= 3);
		if(i <= 2)
			return rgbValue[i];
		return alphaValue;
	}

	ColorRGB rgb() const
	{
		return rgbValue;
	}

	glm::vec4 vector() const
	{
		return {rgbValue.r(), rgbValue.g(), rgbValue.b(), alphaValue};
	}
};

ColorRGBA inline operator+(ColorRGBA const& lhs, ColorRGBA const& rhs)
{
	ColorRGBA result{lhs};
	result += rhs;
	return result;
}

template <typename T>
ColorRGBA operator+(ColorRGBA const& lhs, T const& rhs)
{
	ColorRGBA result{lhs};
	result += rhs;
	return result;
}

template <typename T>
ColorRGBA operator+(T const& lhs, ColorRGBA const& rhs)
{
	ColorRGBA result{rhs};
	result += lhs;
	return result;
}

ColorRGBA inline operator-(ColorRGBA const& lhs, ColorRGBA const& rhs)
{
	ColorRGBA result{lhs};
	result -= rhs;
	return result;
}

template <typename T>
ColorRGBA operator-(ColorRGBA const& lhs, T const& rhs)
{
	ColorRGBA result{lhs};
	result -= rhs;
	return result;
}

ColorRGBA inline operator*(ColorRGBA const& lhs, ColorRGBA const& rhs)
{
	ColorRGBA result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
ColorRGBA operator*(ColorRGBA const& lhs, T const& rhs)
{
	ColorRGBA result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
ColorRGBA operator*(T const& lhs, ColorRGBA const& rhs)
{
	ColorRGBA result{rhs};
	result *= lhs;
	return result;
}

ColorRGBA inline operator/(ColorRGBA const& lhs, ColorRGBA const& rhs)
{
	ColorRGBA result{lhs};
	result /= rhs;
	return result;
}

template <typename T>
ColorRGBA operator/(ColorRGBA const& lhs, T const& rhs)
{
	ColorRGBA result{lhs};
	result /= rhs;
	return result;
}

} // namespace rsp