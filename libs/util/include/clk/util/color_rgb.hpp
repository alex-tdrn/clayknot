#pragma once

#include "clk/util/data_type_name.hpp"

#include <cstddef>
#include <cstdint>
#include <glm/glm.hpp>
#include <string>

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
    auto operator+=(T const& that) -> color_rgb&
    {
        _values += that;
        return *this;
    }

    template <typename T>
    auto operator-=(T const& that) -> color_rgb&
    {
        _values -= that;
        return *this;
    }

    template <typename T>
    auto operator*=(T const& that) -> color_rgb&
    {
        _values *= that;
        return *this;
    }

    template <typename T>
    auto operator/=(T const& that) -> color_rgb&
    {
        _values /= that;
        return *this;
    }
    auto operator+=(color_rgb const& that) -> color_rgb&;
    auto operator-=(color_rgb const& that) -> color_rgb&;
    auto operator*=(color_rgb const& that) -> color_rgb&;
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

template <typename T>
auto operator-(color_rgb const& lhs, T const& rhs) -> color_rgb
{
    color_rgb result{lhs};
    result -= rhs;
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

template <typename T>
auto operator/(color_rgb const& lhs, T const& rhs) -> color_rgb
{
    color_rgb result{lhs};
    result /= rhs;
    return result;
}

auto operator+(color_rgb const& lhs, color_rgb const& rhs) -> color_rgb;

auto operator-(color_rgb const& lhs, color_rgb const& rhs) -> color_rgb;

auto operator*(color_rgb const& lhs, color_rgb const& rhs) -> color_rgb;

auto operator/(color_rgb const& lhs, color_rgb const& rhs) -> color_rgb;

template <>
inline auto data_type_name<color_rgb>::get() -> std::string
{
    return "Color";
}

} // namespace clk
