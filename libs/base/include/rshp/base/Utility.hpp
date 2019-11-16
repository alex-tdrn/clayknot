#pragma once

#include <tuple>

template <class Tup, class F, std::size_t ...I>
constexpr void static_for_impl(Tup&& t, F&& f, std::index_sequence<I...>)
{
	(f(std::get<I>(t)), ...);
}

template <class... T, class F >
constexpr void static_for(std::tuple<T...>& t, F&& f)
{
	static_for_impl(t, std::forward<F>(f), std::make_index_sequence<sizeof...(T)>{});
}

template <class Tup, class F, std::size_t ...I>
constexpr void static_for_impl_index(Tup&& t, F&& f, std::index_sequence<I...>)
{
	(f(std::get<I>(t), I), ...);
}

template <class... T, class F >
constexpr void static_for_index(std::tuple<T...>& t, F&& f)
{
	static_for_impl_index(t, std::forward<F>(f), std::make_index_sequence<sizeof...(T)>{});
}

template <typename...>
struct is_one_of
{
	static constexpr bool value = false;
};

template <typename F, typename S, typename... T>
struct is_one_of<F, S, T...>
{
	static constexpr bool value = std::is_same<F, S>::value || is_one_of<F, T...>::value;
};

template <typename...>
struct is_base_of_any
{
	static constexpr bool value = false;
};

template <typename Base, typename Head, typename... Tail>
struct is_base_of_any<Base, Head, Tail...>
{
	static constexpr bool value = std::is_base_of<Base, Head>::value || is_base_of_any<Base, Tail...>::value;
};
