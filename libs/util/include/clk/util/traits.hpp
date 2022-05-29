#pragma once

#include <type_traits>

namespace clk
{
namespace impl
{
template <template <typename...> class Trait, class Enabler, class... Args>
struct is_detected : std::false_type
{
};

template <template <class...> class Trait, class... Args>
struct is_detected<Trait, std::void_t<Trait<Args...>>, Args...> : std::true_type
{
};

template <typename T>
using has_begin = decltype(std::declval<T>().begin());

template <typename T>
using has_end = decltype(std::declval<T>().end());

} // namespace impl

template <typename T>
constexpr bool is_range_v = impl::is_detected<impl::has_begin, T>::value&& impl::is_detected<impl::has_end, T>::value;

} // namespace clk