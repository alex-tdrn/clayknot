#include "clk/util/color_rgba.hpp"

#include <cassert>
#include <chrono>
#include <cstdint>
#include <random>
#include <unordered_map>

namespace clk
{

color_rgba::color_rgba(glm::vec4 v) : _rgb_value(v.r, v.g, v.b), _alpha_value(v.a)
{
}

color_rgba::color_rgba(color_rgb v) : _rgb_value(v)
{
}

color_rgba::color_rgba(color_rgb v, float a) : _rgb_value(v), _alpha_value(a)
{
}

color_rgba::color_rgba(color_rgb v, int a) : _rgb_value(v), _alpha_value(static_cast<float>(a) / 255.0f)
{
}
color_rgba::color_rgba(float v) : _rgb_value(v), _alpha_value(v)
{
}

color_rgba::color_rgba(int v) : _rgb_value(static_cast<float>(v) / 255.0f), _alpha_value(static_cast<float>(v) / 255.0f)
{
}

color_rgba::color_rgba(float r, float g, float b, float a) : _rgb_value(r, g, b), _alpha_value(a)
{
}
color_rgba::color_rgba(int r, int g, int b, int a)
    : _rgb_value(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
    , _alpha_value(static_cast<float>(a) / 255.0f)
{
}

auto color_rgba::operator==(color_rgba const& that) const -> bool
{
    return _rgb_value == that._rgb_value && _alpha_value == that._alpha_value;
}

auto color_rgba::operator!=(color_rgba const& that) const -> bool
{
    return _rgb_value != that._rgb_value && _alpha_value != that._alpha_value;
}

auto color_rgba::compare_equal_low_precision(color_rgba const& lhs, color_rgba const& rhs) -> bool
{
    return color_rgb::compare_equal_low_precision(lhs._rgb_value, rhs._rgb_value) && lhs.au8() == rhs.au8();
}

auto color_rgba::create_random(std::uint64_t seed) -> color_rgba
{
    static std::unordered_map<std::uint64_t, glm::vec4> cache;
    if(cache.find(seed) == cache.end())
    {
        std::mt19937_64 generator(seed);
        std::uniform_real_distribution<float> dis(0, 1);

        cache[seed] = {dis(generator), dis(generator), dis(generator), dis(generator)};
    }
    return color_rgba{cache[seed]};
}

auto color_rgba::create_random() -> color_rgba
{
    return color_rgba::create_random(std::chrono::system_clock::now().time_since_epoch().count());
}

auto color_rgba::pack(color_rgba color) -> std::uint32_t
{
    std::uint32_t ret = color_rgb::pack(color._rgb_value);
    ret |= color.au8() << 24;
    return ret;
}

auto color_rgba::unpack(std::uint32_t packed_color) -> color_rgba
{
    color_rgb rgb = color_rgb::unpack(packed_color);
    int a = static_cast<int>(packed_color >> 24 & 0b1111'1111);
    return color_rgba{rgb, a};
}

auto color_rgba::valid() const -> bool
{
    if(!_rgb_value.valid())
        return false;
    if(_alpha_value < 0.0f || _alpha_value > 1.0f)
        return false;
    return true;
}

auto color_rgba::operator+=(color_rgba const& that) -> color_rgba&
{
    _rgb_value += that._rgb_value;
    _alpha_value += that._alpha_value;
    return *this;
}

auto color_rgba::operator-=(color_rgba const& that) -> color_rgba&
{
    _rgb_value -= that._rgb_value;
    _alpha_value -= that._alpha_value;
    return *this;
}

auto color_rgba::operator*=(color_rgba const& that) -> color_rgba&
{
    _rgb_value *= that._rgb_value;
    _alpha_value *= that._alpha_value;
    return *this;
}

auto color_rgba::operator/=(color_rgba const& that) -> color_rgba&
{
    _rgb_value /= that._rgb_value;
    _alpha_value /= that._alpha_value;
    return *this;
}

auto color_rgba::data() -> float*
{
    return _rgb_value.data();
}

auto color_rgba::r() const -> float
{
    return _rgb_value.r();
}

auto color_rgba::g() const -> float
{
    return _rgb_value.g();
}

auto color_rgba::b() const -> float
{
    return _rgb_value.b();
}

auto color_rgba::a() const -> float
{
    return _alpha_value;
}

auto color_rgba::ru8() const -> std::uint8_t
{
    return _rgb_value.ru8();
}

auto color_rgba::gu8() const -> std::uint8_t
{
    return _rgb_value.gu8();
}

auto color_rgba::bu8() const -> std::uint8_t
{
    return _rgb_value.bu8();
}

auto color_rgba::au8() const -> std::uint8_t
{
    return static_cast<std::uint8_t>(_alpha_value * 255);
}

auto color_rgba::operator[](std::size_t i) const -> float
{
    assert(i <= 3);
    if(i <= 2)
        return _rgb_value[i];
    return _alpha_value;
}

auto color_rgba::rgb() const -> color_rgb
{
    return _rgb_value;
}

auto color_rgba::vector() const -> glm::vec4
{
    return {_rgb_value.r(), _rgb_value.g(), _rgb_value.b(), _alpha_value};
}

auto color_rgba::packed() const -> std::uint32_t
{
    return color_rgba::pack(*this);
}

auto operator+(color_rgba const& lhs, color_rgba const& rhs) -> color_rgba
{
    color_rgba result{lhs};
    result += rhs;
    return result;
}

auto operator-(color_rgba const& lhs, color_rgba const& rhs) -> color_rgba
{
    color_rgba result{lhs};
    result -= rhs;
    return result;
}

auto operator*(color_rgba const& lhs, color_rgba const& rhs) -> color_rgba
{
    color_rgba result{lhs};
    result *= rhs;
    return result;
}

auto operator/(color_rgba const& lhs, color_rgba const& rhs) -> color_rgba
{
    color_rgba result{lhs};
    result /= rhs;
    return result;
}
} // namespace clk
