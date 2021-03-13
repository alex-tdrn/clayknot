#pragma once

#include "rsp/util/DataTypeName.hpp"

#include <cassert>
#include <chrono>
#include <glm/glm.hpp>
#include <random>
#include <unordered_map>

namespace rsp
{
class ColorRGB
{
public:
	ColorRGB() = default;
	explicit ColorRGB(glm::vec3 v);
	explicit ColorRGB(float v);
	explicit ColorRGB(int v);
	explicit ColorRGB(float r, float g, float b);
	explicit ColorRGB(int r, int g, int b);
	ColorRGB(ColorRGB const&) = default;
	ColorRGB(ColorRGB&&) = default;
	auto operator=(ColorRGB const& that) -> ColorRGB& = default;
	auto operator=(ColorRGB&& that) -> ColorRGB& = default;
	~ColorRGB() = default;
	auto operator==(ColorRGB const& that) const -> bool;
	auto operator!=(ColorRGB const& that) const -> bool;
	static auto compareEqualLowPrecision(ColorRGB const& lhs, ColorRGB const& rhs) -> bool;

	static auto createRandom(std::uint64_t seed) -> ColorRGB;
	static auto createRandom() -> ColorRGB;
	static auto pack(ColorRGB color) -> std::uint32_t;
	static auto unpack(std::uint32_t packedColor) -> ColorRGB;
	auto valid() const -> bool;
	template <typename T>
	auto operator+=(T const& that) -> ColorRGB&;
	auto operator+=(ColorRGB const& that) -> ColorRGB&;
	template <typename T>
	auto operator-=(T const& that) -> ColorRGB&;
	auto operator-=(ColorRGB const& that) -> ColorRGB&;
	template <typename T>
	auto operator*=(T const& that) -> ColorRGB&;
	auto operator*=(ColorRGB const& that) -> ColorRGB&;
	template <typename T>
	auto operator/=(T const& that) -> ColorRGB&;
	auto operator/=(ColorRGB const& that) -> ColorRGB&;
	auto data() -> float*;
	auto r() const -> float;
	auto g() const -> float;
	auto b() const -> float;
	auto ru8() const -> std::uint8_t;
	auto gu8() const -> std::uint8_t;
	auto bu8() const -> std::uint8_t;
	auto operator[](std::size_t i) const -> float;
	auto vector() const -> glm::vec3;
	auto packed() const -> std::uint32_t;

private:
	glm::vec3 values = {0.0f, 0.0f, 0.0f};
};

inline ColorRGB::ColorRGB(glm::vec3 v) : values(v)
{
}

inline ColorRGB::ColorRGB(float v) : values(v)
{
}

inline ColorRGB::ColorRGB(int v) : values(static_cast<float>(v) / 255.0f)
{
}

inline ColorRGB::ColorRGB(float r, float g, float b) : values(r, g, b)
{
}

inline ColorRGB::ColorRGB(int r, int g, int b)
	: values(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
{
}

inline auto ColorRGB::operator==(ColorRGB const& that) const -> bool
{
	return values == that.values;
}

inline auto ColorRGB::operator!=(ColorRGB const& that) const -> bool
{
	return values != that.values;
}

inline auto ColorRGB::compareEqualLowPrecision(ColorRGB const& lhs, ColorRGB const& rhs) -> bool
{
	return lhs.ru8() == rhs.ru8() && lhs.gu8() == rhs.gu8() && lhs.bu8() == rhs.bu8();
}

inline auto ColorRGB::createRandom(std::uint64_t seed) -> ColorRGB
{
	static std::unordered_map<std::uint64_t, glm::vec3> cache;
	if(cache.find(seed) == cache.end())
	{
		std::mt19937 generator(seed);
		std::uniform_real_distribution<float> dis(0, 1);

		cache[seed] = {dis(generator), dis(generator), dis(generator)};
	}
	return ColorRGB{cache[seed]};
}

inline auto ColorRGB::createRandom() -> ColorRGB
{
	return ColorRGB::createRandom(std::chrono::system_clock::now().time_since_epoch().count());
}

inline auto ColorRGB::pack(ColorRGB color) -> std::uint32_t
{
	std::uint32_t ret = color.ru8();
	ret |= color.gu8() << 8;
	ret |= color.bu8() << 16;
	return ret;
}

inline auto ColorRGB::unpack(std::uint32_t packedColor) -> ColorRGB
{
	std::uint32_t mask = 0b1111'1111;

	int r = static_cast<int>(packedColor & mask);
	int g = static_cast<int>(packedColor >> 8 & mask);
	int b = static_cast<int>(packedColor >> 16 & mask);
	return ColorRGB{r, g, b};
}

inline auto ColorRGB::valid() const -> bool
{
	for(int i = 0; i <= 2; i++)
		if(values[i] < 0)
			return false;
	return true;
}

template <typename T>
auto ColorRGB::operator+=(T const& that) -> ColorRGB&
{
	this->values += that;
	assert(valid());
	return *this;
}

inline auto ColorRGB::operator+=(ColorRGB const& that) -> ColorRGB&
{
	this->values += that.values;
	assert(valid());
	return *this;
}

template <typename T>
auto ColorRGB::operator-=(T const& that) -> ColorRGB&
{
	this->values -= that;
	assert(valid());
	return *this;
}

inline auto ColorRGB::operator-=(ColorRGB const& that) -> ColorRGB&
{
	this->values -= that.values;
	assert(valid());
	return *this;
}

template <typename T>
auto ColorRGB::operator*=(T const& that) -> ColorRGB&
{
	this->values *= that;
	assert(valid());
	return *this;
}

inline auto ColorRGB::operator*=(ColorRGB const& that) -> ColorRGB&
{
	this->values *= that.values;
	assert(valid());
	return *this;
}

template <typename T>
auto ColorRGB::operator/=(T const& that) -> ColorRGB&
{
	this->values /= that;
	assert(valid());
	return *this;
}

inline auto ColorRGB::operator/=(ColorRGB const& that) -> ColorRGB&
{
	this->values /= that.values;
	assert(valid());
	return *this;
}

inline auto ColorRGB::data() -> float*
{
	return &values.r;
}

inline auto ColorRGB::r() const -> float
{
	return values.r;
}

inline auto ColorRGB::g() const -> float
{
	return values.g;
}

inline auto ColorRGB::b() const -> float
{
	return values.b;
}

inline auto ColorRGB::ru8() const -> std::uint8_t
{
	return static_cast<std::uint8_t>(values.r * 255);
}

inline auto ColorRGB::gu8() const -> std::uint8_t
{
	return static_cast<std::uint8_t>(values.g * 255);
}

inline auto ColorRGB::bu8() const -> std::uint8_t
{
	return static_cast<std::uint8_t>(values.b * 255);
}

inline auto ColorRGB::operator[](std::size_t i) const -> float
{
	assert(i <= 2);
	return values[i];
}

inline auto ColorRGB::vector() const -> glm::vec3
{
	return values;
}

inline auto ColorRGB::packed() const -> std::uint32_t
{
	return ColorRGB::pack(*this);
}

auto inline operator+(ColorRGB const& lhs, ColorRGB const& rhs) -> ColorRGB
{
	ColorRGB result{lhs};
	result += rhs;
	return result;
}

template <typename T>
auto operator+(ColorRGB const& lhs, T const& rhs) -> ColorRGB
{
	ColorRGB result{lhs};
	result += rhs;
	return result;
}

template <typename T>
auto operator+(T const& lhs, ColorRGB const& rhs) -> ColorRGB
{
	ColorRGB result{rhs};
	result += lhs;
	return result;
}

auto inline operator-(ColorRGB const& lhs, ColorRGB const& rhs) -> ColorRGB
{
	ColorRGB result{lhs};
	result -= rhs;
	return result;
}

template <typename T>
auto operator-(ColorRGB const& lhs, T const& rhs) -> ColorRGB
{
	ColorRGB result{lhs};
	result -= rhs;
	return result;
}

auto inline operator*(ColorRGB const& lhs, ColorRGB const& rhs) -> ColorRGB
{
	ColorRGB result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
auto operator*(ColorRGB const& lhs, T const& rhs) -> ColorRGB
{
	ColorRGB result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
auto operator*(T const& lhs, ColorRGB const& rhs) -> ColorRGB
{
	ColorRGB result{rhs};
	result *= lhs;
	return result;
}

auto inline operator/(ColorRGB const& lhs, ColorRGB const& rhs) -> ColorRGB
{
	ColorRGB result{lhs};
	result /= rhs;
	return result;
}

template <typename T>
auto operator/(ColorRGB const& lhs, T const& rhs) -> ColorRGB
{
	ColorRGB result{lhs};
	result /= rhs;
	return result;
}

template <>
inline auto DataTypeName<ColorRGB>::get() -> std::string
{
	return "Color";
}

} // namespace rsp