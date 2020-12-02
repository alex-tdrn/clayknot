#pragma once

#include "rsp/util/DataTypeName.hpp"

#include <cassert>
#include <glm/glm.hpp>

namespace rsp
{
class ColorRGB
{
public:
	ColorRGB() = default;
	explicit ColorRGB(glm::vec3 v);
	explicit ColorRGB(float v);
	explicit ColorRGB(float r, float g, float b);
	explicit ColorRGB(int r, int g, int b);
	ColorRGB(ColorRGB const&) = default;
	ColorRGB(ColorRGB&&) = default;
	auto operator=(ColorRGB const& that) -> ColorRGB& = default;
	auto operator=(ColorRGB&& that) -> ColorRGB& = default;
	~ColorRGB() = default;

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
	auto operator[](std::size_t i) const -> float;
	auto vector() const -> glm::vec3;

private:
	glm::vec3 values = {0.0f, 0.0f, 0.0f};
};

inline ColorRGB::ColorRGB(glm::vec3 v) : values(v)
{
}

inline ColorRGB::ColorRGB(float v) : values(v)
{
}

inline ColorRGB::ColorRGB(float r, float g, float b) : values(r, g, b)
{
}

inline ColorRGB::ColorRGB(int r, int g, int b)
	: values(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
{
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

inline auto ColorRGB::operator[](std::size_t i) const -> float
{
	assert(i <= 2);
	return values[i];
}

inline auto ColorRGB::vector() const -> glm::vec3
{
	return values;
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