#pragma once

#include "clk/util/color_rgb.hpp"
#include "clk/util/data_type_name.hpp"

#include <cstddef>
#include <cstdint>
#include <glm/glm.hpp>
#include <string>

namespace clk
{
class color_rgba
{
public:
	color_rgba() = default;
	explicit color_rgba(glm::vec4 v);
	explicit color_rgba(color_rgb v);
	explicit color_rgba(color_rgb v, float a);
	explicit color_rgba(color_rgb v, int a);
	explicit color_rgba(float v);
	explicit color_rgba(int v);
	explicit color_rgba(float r, float g, float b, float a);
	explicit color_rgba(int r, int g, int b, int a);
	color_rgba(color_rgba const&) = default;
	color_rgba(color_rgba&&) = default;
	auto operator=(color_rgba const& that) -> color_rgba& = default;
	auto operator=(color_rgba&& that) -> color_rgba& = default;
	~color_rgba() = default;
	auto operator==(color_rgba const& that) const -> bool;
	auto operator!=(color_rgba const& that) const -> bool;
	static auto compare_equal_low_precision(color_rgba const& lhs, color_rgba const& rhs) -> bool;

	static auto create_random(std::uint64_t seed) -> color_rgba;
	static auto create_random() -> color_rgba;
	static auto pack(color_rgba color) -> std::uint32_t;
	static auto unpack(std::uint32_t packed_color) -> color_rgba;
	auto valid() const -> bool;

	template <typename T>
	auto operator+=(T const& that) -> color_rgba&
	{
		_rgb_value += that;
		_alpha_value += that;
		return *this;
	}

	template <typename T>
	auto operator-=(T const& that) -> color_rgba&
	{
		_rgb_value -= that;
		_alpha_value -= that;
		return *this;
	}

	template <typename T>
	auto operator*=(T const& that) -> color_rgba&
	{
		_rgb_value *= that;
		_alpha_value *= that;
		return *this;
	}

	template <typename T>
	auto operator/=(T const& that) -> color_rgba&
	{
		_rgb_value /= that;
		_alpha_value /= that;
		return *this;
	}

	auto operator+=(color_rgba const& that) -> color_rgba&;
	auto operator-=(color_rgba const& that) -> color_rgba&;
	auto operator*=(color_rgba const& that) -> color_rgba&;
	auto operator/=(color_rgba const& that) -> color_rgba&;

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
	auto rgb() const -> color_rgb;
	auto vector() const -> glm::vec4;
	auto packed() const -> std::uint32_t;

private:
	color_rgb _rgb_value;
	float _alpha_value = 1.0f;
};

template <typename T>
auto operator+(color_rgba const& lhs, T const& rhs) -> color_rgba
{
	color_rgba result{lhs};
	result += rhs;
	return result;
}

template <typename T>
auto operator+(T const& lhs, color_rgba const& rhs) -> color_rgba
{
	color_rgba result{rhs};
	result += lhs;
	return result;
}

template <typename T>
auto operator-(color_rgba const& lhs, T const& rhs) -> color_rgba
{
	color_rgba result{lhs};
	result -= rhs;
	return result;
}

template <typename T>
auto operator*(color_rgba const& lhs, T const& rhs) -> color_rgba
{
	color_rgba result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
auto operator*(T const& lhs, color_rgba const& rhs) -> color_rgba
{
	color_rgba result{rhs};
	result *= lhs;
	return result;
}

template <typename T>
auto operator/(color_rgba const& lhs, T const& rhs) -> color_rgba
{
	color_rgba result{lhs};
	result /= rhs;
	return result;
}

auto operator+(color_rgba const& lhs, color_rgba const& rhs) -> color_rgba;

auto operator-(color_rgba const& lhs, color_rgba const& rhs) -> color_rgba;

auto operator*(color_rgba const& lhs, color_rgba const& rhs) -> color_rgba;

auto operator/(color_rgba const& lhs, color_rgba const& rhs) -> color_rgba;

template <>
inline auto data_type_name<color_rgba>::get() -> std::string
{
	return "Color with transparency";
}
} // namespace clk