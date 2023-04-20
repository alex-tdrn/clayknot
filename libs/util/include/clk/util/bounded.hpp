#pragma once

#include "clk/util/data_type_name.hpp"

#include <cassert>
#include <glm/glm.hpp>

namespace clk
{
template <typename T>
class bounded
{
public:
    explicit bounded(T val, T min, T max) : _val(val), _min(min), _max(max)
    {
        assert(valid());
    }

    bounded() = default;
    bounded(bounded const&) = default;
    bounded(bounded&&) noexcept = default;
    auto operator=(bounded const& that) -> bounded& = default;
    auto operator=(bounded&& that) noexcept -> bounded& = default;

    auto operator=(T const& that) -> bounded&
    {
        _val = that;
        assert(valid());
    }

    auto operator=(T&& that) -> bounded&
    {
        _val = std::move(that);
        assert(valid());
    }

    ~bounded() = default;

    auto valid() -> bool
    {
        return _min <= _max && _val >= _min && _val <= _max;
    }

    void set_min(T min)
    {
        _min = min;
        assert(valid());
    }

    void set_max(T max)
    {
        _max = max;
        assert(valid());
    }

    auto min() const -> T
    {
        return _min;
    }

    auto max() const -> T
    {
        return _max;
    }

    auto val() const -> T
    {
        return _val;
    }

    auto data() -> T*
    {
        return &_val;
    }

    operator T() const
    {
        return _val;
    }

    auto operator+=(T const& that) -> bounded&
    {
        _val += that;
        assert(valid());
        return *this;
    }

    auto operator-=(T const& that) -> bounded&
    {
        _val -= that;
        assert(valid());
        return *this;
    }

    auto operator*=(T const& that) -> bounded&
    {
        _val *= that;
        assert(valid());
        return *this;
    }

    auto operator/=(T const& that) -> bounded&
    {
        _val /= that;
        assert(valid());
        return *this;
    }

private:
    T _val;
    T _min;
    T _max;
};

template <>
inline auto bounded<glm::vec2>::valid() -> bool
{
    for(int i = 0; i < 2; i++)
    {
        if(_min[i] > _max[i] || _val[i] < _min[i] || _val[i] > _max[i])
            return false;
    }
    return true;
}

template <>
inline auto bounded<glm::vec3>::valid() -> bool
{
    for(int i = 0; i < 3; i++)
    {
        if(_min[i] > _max[i] || _val[i] < _min[i] || _val[i] > _max[i])
            return false;
    }
    return true;
}

template <>
inline auto bounded<glm::vec4>::valid() -> bool
{
    for(int i = 0; i < 4; i++)
    {
        if(_min[i] > _max[i] || _val[i] < _min[i] || _val[i] > _max[i])
            return false;
    }
    return true;
}

template <typename DataType>
struct data_type_name<bounded<DataType>>
{
    static auto get() -> std::string
    {
        return "bounded " + data_type_name<DataType>::get();
    }
};

} // namespace clk
