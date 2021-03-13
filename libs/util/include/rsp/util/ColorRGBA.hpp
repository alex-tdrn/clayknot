#pragma once

#include "rsp/util/ColorRGB.hpp"

#include <cassert>

namespace rsp
{
class ColorRGBA
{
public:
	ColorRGBA() = default;
	explicit ColorRGBA(glm::vec4 v);
	explicit ColorRGBA(ColorRGB v);
	explicit ColorRGBA(ColorRGB v, float a);
	explicit ColorRGBA(ColorRGB v, int a);
	explicit ColorRGBA(float v);
	explicit ColorRGBA(int v);
	explicit ColorRGBA(float r, float g, float b, float a);
	explicit ColorRGBA(int r, int g, int b, int a);
	ColorRGBA(ColorRGBA const&) = default;
	ColorRGBA(ColorRGBA&&) = default;
	auto operator=(ColorRGBA const& that) -> ColorRGBA& = default;
	auto operator=(ColorRGBA&& that) -> ColorRGBA& = default;
	~ColorRGBA() = default;
	auto operator==(ColorRGBA const& that) const -> bool;
	auto operator!=(ColorRGBA const& that) const -> bool;
	static auto compareEqualLowPrecision(ColorRGBA const& lhs, ColorRGBA const& rhs) -> bool;

	static auto createRandom(std::uint64_t seed) -> ColorRGBA;
	static auto createRandom() -> ColorRGBA;
	static auto pack(ColorRGBA color) -> std::uint32_t;
	static auto unpack(std::uint32_t packedColor) -> ColorRGBA;
	auto valid() const -> bool;
	template <typename T>
	auto operator+=(T const& that) -> ColorRGBA&;
	auto operator+=(ColorRGBA const& that) -> ColorRGBA&;
	template <typename T>
	auto operator-=(T const& that) -> ColorRGBA&;
	auto operator-=(ColorRGBA const& that) -> ColorRGBA&;
	template <typename T>
	auto operator*=(T const& that) -> ColorRGBA&;
	auto operator*=(ColorRGBA const& that) -> ColorRGBA&;
	template <typename T>
	auto operator/=(T const& that) -> ColorRGBA&;
	auto operator/=(ColorRGBA const& that) -> ColorRGBA&;
	auto data() -> float*;
	auto r() const -> float;
	auto g() const -> float;
	auto b() const -> float;
	auto a() const -> float;
	auto ru8() const -> std::uint8_t;
	auto gu8() const -> std::uint8_t;
	auto bu8() const -> std::uint8_t;
	auto au8() const -> std::uint8_t;
	auto operator[](std::size_t i) const -> float;
	auto rgb() const -> ColorRGB;
	auto vector() const -> glm::vec4;
	auto packed() const -> std::uint32_t;

private:
	ColorRGB rgbValue;
	float alphaValue = 1.0f;
};

inline ColorRGBA::ColorRGBA(glm::vec4 v) : rgbValue(v.r, v.g, v.b), alphaValue(v.a)
{
}

inline ColorRGBA::ColorRGBA(ColorRGB v) : rgbValue(v)
{
}

inline ColorRGBA::ColorRGBA(ColorRGB v, float a) : rgbValue(v), alphaValue(a)
{
}

inline ColorRGBA::ColorRGBA(ColorRGB v, int a) : rgbValue(v), alphaValue(static_cast<float>(a) / 255.0f)
{
}
inline ColorRGBA::ColorRGBA(float v) : rgbValue(v), alphaValue(v)
{
}

inline ColorRGBA::ColorRGBA(int v)
	: rgbValue(static_cast<float>(v) / 255.0f), alphaValue(static_cast<float>(v) / 255.0f)
{
}

inline ColorRGBA::ColorRGBA(float r, float g, float b, float a) : rgbValue(r, g, b), alphaValue(a)
{
}
inline ColorRGBA::ColorRGBA(int r, int g, int b, int a)
	: rgbValue(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
	, alphaValue(static_cast<float>(a) / 255.0f)
{
}

inline auto ColorRGBA::operator==(ColorRGBA const& that) const -> bool
{
	return rgbValue == that.rgbValue && alphaValue == that.alphaValue;
}

inline auto ColorRGBA::operator!=(ColorRGBA const& that) const -> bool
{
	return rgbValue != that.rgbValue && alphaValue != that.alphaValue;
}

inline auto ColorRGBA::compareEqualLowPrecision(ColorRGBA const& lhs, ColorRGBA const& rhs) -> bool
{
	return ColorRGB::compareEqualLowPrecision(lhs.rgbValue, rhs.rgbValue) && lhs.au8() == rhs.au8();
}

inline auto ColorRGBA::createRandom(std::uint64_t seed) -> ColorRGBA
{
	static std::unordered_map<std::uint64_t, glm::vec4> cache;
	if(cache.find(seed) == cache.end())
	{
		std::mt19937 generator(seed);
		std::uniform_real_distribution<float> dis(0, 1);

		cache[seed] = {dis(generator), dis(generator), dis(generator), dis(generator)};
	}
	return ColorRGBA{cache[seed]};
}

inline auto ColorRGBA::createRandom() -> ColorRGBA
{
	return ColorRGBA::createRandom(std::chrono::system_clock::now().time_since_epoch().count());
}

inline auto ColorRGBA::pack(ColorRGBA color) -> std::uint32_t
{
	std::uint32_t ret = ColorRGB::pack(color.rgbValue);
	ret |= color.au8() << 24;
	return ret;
}

inline auto ColorRGBA::unpack(std::uint32_t packedColor) -> ColorRGBA
{
	ColorRGB rgb = ColorRGB::unpack(packedColor);
	int a = static_cast<int>(packedColor >> 24 & 0b1111'1111);
	return ColorRGBA{rgb, a};
}

inline auto ColorRGBA::valid() const -> bool
{
	if(!rgbValue.valid())
		return false;
	if(alphaValue < 0.0f || alphaValue > 1.0f)
		return false;
	return true;
}

template <typename T>
auto ColorRGBA::operator+=(T const& that) -> ColorRGBA&
{
	this->rgbValue += that;
	this->alphaValue += that;
	assert(valid());
	return *this;
}

inline auto ColorRGBA::operator+=(ColorRGBA const& that) -> ColorRGBA&
{
	this->rgbValue += that.rgbValue;
	this->alphaValue += that.alphaValue;
	assert(valid());
	return *this;
}

template <typename T>
auto ColorRGBA::operator-=(T const& that) -> ColorRGBA&
{
	this->rgbValue -= that;
	this->alphaValue -= that;
	assert(valid());
	return *this;
}

inline auto ColorRGBA::operator-=(ColorRGBA const& that) -> ColorRGBA&
{
	this->rgbValue -= that.rgbValue;
	this->alphaValue -= that.alphaValue;
	assert(valid());
	return *this;
}

template <typename T>
auto ColorRGBA::operator*=(T const& that) -> ColorRGBA&
{
	this->rgbValue *= that;
	this->alphaValue *= that;
	assert(valid());
	return *this;
}

inline auto ColorRGBA::operator*=(ColorRGBA const& that) -> ColorRGBA&
{
	this->rgbValue *= that.rgbValue;
	this->alphaValue *= that.alphaValue;
	assert(valid());
	return *this;
}

template <typename T>
auto ColorRGBA::operator/=(T const& that) -> ColorRGBA&
{
	this->rgbValue /= that;
	this->alphaValue /= that;
	assert(valid());
	return *this;
}

inline auto ColorRGBA::operator/=(ColorRGBA const& that) -> ColorRGBA&
{
	this->rgbValue /= that.rgbValue;
	this->alphaValue /= that.alphaValue;
	assert(valid());
	return *this;
}

inline auto ColorRGBA::data() -> float*
{
	return rgbValue.data();
}

inline auto ColorRGBA::r() const -> float
{
	return rgbValue.r();
}

inline auto ColorRGBA::g() const -> float
{
	return rgbValue.g();
}

inline auto ColorRGBA::b() const -> float
{
	return rgbValue.b();
}

inline auto ColorRGBA::a() const -> float
{
	return alphaValue;
}

inline auto ColorRGBA::ru8() const -> std::uint8_t
{
	return rgbValue.ru8();
}

inline auto ColorRGBA::gu8() const -> std::uint8_t
{
	return rgbValue.gu8();
}

inline auto ColorRGBA::bu8() const -> std::uint8_t
{
	return rgbValue.bu8();
}

inline auto ColorRGBA::au8() const -> std::uint8_t
{
	return static_cast<std::uint8_t>(alphaValue * 255);
}

inline auto ColorRGBA::operator[](std::size_t i) const -> float
{
	assert(i <= 3);
	if(i <= 2)
		return rgbValue[i];
	return alphaValue;
}

inline auto ColorRGBA::rgb() const -> ColorRGB
{
	return rgbValue;
}

inline auto ColorRGBA::vector() const -> glm::vec4
{
	return {rgbValue.r(), rgbValue.g(), rgbValue.b(), alphaValue};
}

inline auto ColorRGBA::packed() const -> std::uint32_t
{
	return ColorRGBA::pack(*this);
}

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