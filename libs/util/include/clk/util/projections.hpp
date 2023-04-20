#pragma once
namespace clk::projections
{
static constexpr auto underlying()
{
    return [](auto const& wrapper) {
        return wrapper.get();
    };
}

template <typename T>
static constexpr auto cast()
{
    return [](auto object) {
        return dynamic_cast<T>(object);
    };
}

} // namespace clk::projections
