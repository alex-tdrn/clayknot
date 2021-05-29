#pragma once

#include "clk/util/data_type_name.hpp"

#include <cassert>
#include <chrono>
#include <glm/glm.hpp>
#include <random>
#include <unordered_map>

namespace clk
{
class color_rgb
{
public:
	color_rgb() = default;
	explicit color_rgb(glm::vec3 v);
	explicit color_rgb(float v);
	explicit color_rgb(int v);
	explicit color_rgb(float r, float g, float b);
	explicit color_rgb(int r, int g, int b);
	color_rgb(color_rgb const&) = default;
	color_rgb(color_rgb&&) = default;
	auto operator=(color_rgb const& that) -> color_rgb& = default;
	auto operator=(color_rgb&& that) -> color_rgb& = default;
	~color_rgb() = default;
	auto operator==(color_rgb const& that) const -> bool;
	auto operator!=(color_rgb const& that) const -> bool;
	static auto compare_equal_low_precision(color_rgb const& lhs, color_rgb const& rhs) -> bool;

	static auto create_random(std::uint64_t seed) -> color_rgb;
	static auto create_random() -> color_rgb;
	static auto pack(color_rgb color) -> std::uint32_t;
	static auto unpack(std::uint32_t packed_color) -> color_rgb;
	auto valid() const -> bool;
	template <typename T>
	auto operator+=(T const& that) -> color_rgb&;
	auto operator+=(color_rgb const& that) -> color_rgb&;
	template <typename T>
	auto operator-=(T const& that) -> color_rgb&;
	auto operator-=(color_rgb const& that) -> color_rgb&;
	template <typename T>
	auto operator*=(T const& that) -> color_rgb&;
	auto operator*=(color_rgb const& that) -> color_rgb&;
	template <typename T>
	auto operator/=(T const& that) -> color_rgb&;
	auto operator/=(color_rgb const& that) -> color_rgb&;
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
	glm::vec3 _values = {0.0f, 0.0f, 0.0f};
};

inline color_rgb::color_rgb(glm::vec3 v) : _values(v)
{
}

inline color_rgb::color_rgb(float v) : _values(v)
{
}

inline color_rgb::color_rgb(int v) : _values(static_cast<float>(v) / 255.0f)
{
}

inline color_rgb::color_rgb(float r, float g, float b) : _values(r, g, b)
{
}

inline color_rgb::color_rgb(int r, int g, int b)
	: _values(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
{
}

inline auto color_rgb::operator==(color_rgb const& that) const -> bool
{
	return _values == that._values;
}

inline auto color_rgb::operator!=(color_rgb const& that) const -> bool
{
	return _values != that._values;
}

inline auto color_rgb::compare_equal_low_precision(color_rgb const& lhs, color_rgb const& rhs) -> bool
{
	return lhs.ru8() == rhs.ru8() && lhs.gu8() == rhs.gu8() && lhs.bu8() == rhs.bu8();
}

inline auto color_rgb::create_random(std::uint64_t seed) -> color_rgb
{
	static std::unordered_map<std::uint64_t, glm::vec3> cache;
	if(cache.find(seed) == cache.end())
	{
		std::mt19937_64 generator(seed);
		std::uniform_real_distribution<float> dis(0, 1);

		cache[seed] = {dis(generator), dis(generator), dis(generator)};
	}
	return color_rgb{cache[seed]};
}

inline auto color_rgb::create_random() -> color_rgb
{
	return color_rgb::create_random(std::chrono::system_clock::now().time_since_epoch().count());
}

inline auto color_rgb::pack(color_rgb color) -> std::uint32_t
{
	std::uint32_t ret = color.ru8();
	ret |= color.gu8() << 8;
	ret |= color.bu8() << 16;
	return ret;
}

inline auto color_rgb::unpack(std::uint32_t packed_color) -> color_rgb
{
	std::uint32_t mask = 0b1111'1111;

	int r = static_cast<int>(packed_color & mask);
	int g = static_cast<int>(packed_color >> 8 & mask);
	int b = static_cast<int>(packed_color >> 16 & mask);
	return color_rgb{r, g, b};
}

inline auto color_rgb::valid() const -> bool
{
	for(int i = 0; i <= 2; i++)
		if(_values[i] < 0)
			return false;
	return true;
}

template <typename T>
auto color_rgb::operator+=(T const& that) -> color_rgb&
{
	_values += that;
	return *this;
}

inline auto color_rgb::operator+=(color_rgb const& that) -> color_rgb&
{
	_values += that._values;
	return *this;
}

template <typename T>
auto color_rgb::operator-=(T const& that) -> color_rgb&
{
	_values -= that;
	return *this;
}

inline auto color_rgb::operator-=(color_rgb const& that) -> color_rgb&
{
	_values -= that._values;
	return *this;
}

template <typename T>
auto color_rgb::operator*=(T const& that) -> color_rgb&
{
	_values *= that;
	return *this;
}

inline auto color_rgb::operator*=(color_rgb const& that) -> color_rgb&
{
	_values *= that._values;
	return *this;
}

template <typename T>
auto color_rgb::operator/=(T const& that) -> color_rgb&
{
	_values /= that;
	return *this;
}

inline auto color_rgb::operator/=(color_rgb const& that) -> color_rgb&
{
	_values /= that._values;
	return *this;
}

inline auto color_rgb::data() -> float*
{
	return &_values.r;
}

inline auto color_rgb::r() const -> float
{
	return _values.r;
}

inline auto color_rgb::g() const -> float
{
	return _values.g;
}

inline auto color_rgb::b() const -> float
{
	return _values.b;
}

inline auto color_rgb::ru8() const -> std::uint8_t
{
	return static_cast<std::uint8_t>(_values.r * 255);
}

inline auto color_rgb::gu8() const -> std::uint8_t
{
	return static_cast<std::uint8_t>(_values.g * 255);
}

inline auto color_rgb::bu8() const -> std::uint8_t
{
	return static_cast<std::uint8_t>(_values.b * 255);
}

inline auto color_rgb::operator[](std::size_t i) const -> float
{
	assert(i <= 2);
	return _values[static_cast<int>(i)];
}

inline auto color_rgb::vector() const -> glm::vec3
{
	return _values;
}

inline auto color_rgb::packed() const -> std::uint32_t
{
	return color_rgb::pack(*this);
}

auto inline operator+(color_rgb const& lhs, color_rgb const& rhs) -> color_rgb
{
	color_rgb result{lhs};
	result += rhs;
	return result;
}

template <typename T>
auto operator+(color_rgb const& lhs, T const& rhs) -> color_rgb
{
	color_rgb result{lhs};
	result += rhs;
	return result;
}

template <typename T>
auto operator+(T const& lhs, color_rgb const& rhs) -> color_rgb
{
	color_rgb result{rhs};
	result += lhs;
	return result;
}

auto inline operator-(color_rgb const& lhs, color_rgb const& rhs) -> color_rgb
{
	color_rgb result{lhs};
	result -= rhs;
	return result;
}

template <typename T>
auto operator-(color_rgb const& lhs, T const& rhs) -> color_rgb
{
	color_rgb result{lhs};
	result -= rhs;
	return result;
}

auto inline operator*(color_rgb const& lhs, color_rgb const& rhs) -> color_rgb
{
	color_rgb result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
auto operator*(color_rgb const& lhs, T const& rhs) -> color_rgb
{
	color_rgb result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
auto operator*(T const& lhs, color_rgb const& rhs) -> color_rgb
{
	color_rgb result{rhs};
	result *= lhs;
	return result;
}

auto inline operator/(color_rgb const& lhs, color_rgb const& rhs) -> color_rgb
{
	color_rgb result{lhs};
	result /= rhs;
	return result;
}

template <typename T>
auto operator/(color_rgb const& lhs, T const& rhs) -> color_rgb
{
	color_rgb result{lhs};
	result /= rhs;
	return result;
}

template <>
inline auto data_type_name<color_rgb>::get() -> std::string
{
	return "Color";
}

} // namespace clk