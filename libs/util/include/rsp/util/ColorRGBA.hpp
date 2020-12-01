#pragma once

#include "rsp/util/ColorRGB.hpp"

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
		: rgbValue(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
		, alphaValue(static_cast<float>(a) / 255.0f)
	{
	}
	ColorRGBA(ColorRGBA const&) = default;
	ColorRGBA(ColorRGBA&&) = default;
	auto operator=(ColorRGBA const& that) -> ColorRGBA& = default;
	auto operator=(ColorRGBA&& that) -> ColorRGBA& = default;
	~ColorRGBA() = default;

public:
	auto valid() const -> bool
	{
		if(!rgbValue.valid())
			return false;
		if(alphaValue < 0.0f || alphaValue > 1.0f)
			return false;
		return true;
	}

	template <typename T>
	auto operator+=(T const& that) -> ColorRGBA&
	{
		this->rgbValue += that;
		this->alphaValue += that;
		assert(valid());
		return *this;
	}

	auto operator+=(ColorRGBA const& that) -> ColorRGBA&
	{
		this->rgbValue += that.rgbValue;
		this->alphaValue += that.alphaValue;
		assert(valid());
		return *this;
	}

	template <typename T>
	auto operator-=(T const& that) -> ColorRGBA&
	{
		this->rgbValue -= that;
		this->alphaValue -= that;
		assert(valid());
		return *this;
	}

	auto operator-=(ColorRGBA const& that) -> ColorRGBA&
	{
		this->rgbValue -= that.rgbValue;
		this->alphaValue -= that.alphaValue;
		assert(valid());
		return *this;
	}

	template <typename T>
	auto operator*=(T const& that) -> ColorRGBA&
	{
		this->rgbValue *= that;
		this->alphaValue *= that;
		assert(valid());
		return *this;
	}

	auto operator*=(ColorRGBA const& that) -> ColorRGBA&
	{
		this->rgbValue *= that.rgbValue;
		this->alphaValue *= that.alphaValue;
		assert(valid());
		return *this;
	}

	template <typename T>
	auto operator/=(T const& that) -> ColorRGBA&
	{
		this->rgbValue /= that;
		this->alphaValue /= that;
		assert(valid());
		return *this;
	}

	auto operator/=(ColorRGBA const& that) -> ColorRGBA&
	{
		this->rgbValue /= that.rgbValue;
		this->alphaValue /= that.alphaValue;
		assert(valid());
		return *this;
	}

	auto data() -> float*
	{
		return rgbValue.data();
	}

	auto r() const -> float
	{
		return rgbValue.r();
	}

	auto g() const -> float
	{
		return rgbValue.g();
	}

	auto b() const -> float
	{
		return rgbValue.b();
	}

	auto a() const -> float
	{
		return alphaValue;
	}

	auto operator[](std::size_t i) const -> float
	{
		assert(i <= 3);
		if(i <= 2)
			return rgbValue[i];
		return alphaValue;
	}

	auto rgb() const -> ColorRGB
	{
		return rgbValue;
	}

	auto vector() const -> glm::vec4
	{
		return {rgbValue.r(), rgbValue.g(), rgbValue.b(), alphaValue};
	}
};

auto inline operator+(ColorRGBA const& lhs, ColorRGBA const& rhs) -> ColorRGBA
{
	ColorRGBA result{lhs};
	result += rhs;
	return result;
}

template <typename T>
auto operator+(ColorRGBA const& lhs, T const& rhs) -> ColorRGBA
{
	ColorRGBA result{lhs};
	result += rhs;
	return result;
}

template <typename T>
auto operator+(T const& lhs, ColorRGBA const& rhs) -> ColorRGBA
{
	ColorRGBA result{rhs};
	result += lhs;
	return result;
}

auto inline operator-(ColorRGBA const& lhs, ColorRGBA const& rhs) -> ColorRGBA
{
	ColorRGBA result{lhs};
	result -= rhs;
	return result;
}

template <typename T>
auto operator-(ColorRGBA const& lhs, T const& rhs) -> ColorRGBA
{
	ColorRGBA result{lhs};
	result -= rhs;
	return result;
}

auto inline operator*(ColorRGBA const& lhs, ColorRGBA const& rhs) -> ColorRGBA
{
	ColorRGBA result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
auto operator*(ColorRGBA const& lhs, T const& rhs) -> ColorRGBA
{
	ColorRGBA result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
auto operator*(T const& lhs, ColorRGBA const& rhs) -> ColorRGBA
{
	ColorRGBA result{rhs};
	result *= lhs;
	return result;
}

auto inline operator/(ColorRGBA const& lhs, ColorRGBA const& rhs) -> ColorRGBA
{
	ColorRGBA result{lhs};
	result /= rhs;
	return result;
}

template <typename T>
auto operator/(ColorRGBA const& lhs, T const& rhs) -> ColorRGBA
{
	ColorRGBA result{lhs};
	result /= rhs;
	return result;
}

template <>
inline auto DataTypeName<ColorRGBA>::get() -> std::string
{
	return "Color with transparency";
}

} // namespace rsp