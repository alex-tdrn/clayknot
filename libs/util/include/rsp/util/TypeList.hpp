#pragma once

#include <tuple>

namespace rsp::meta
{
template <typename... Types>
class TypeList
{
public:
	template <std::size_t Index>
	using get = std::tuple_element_t<Index, std::tuple<Types...>>;
	static constexpr std::size_t size = sizeof...(Types);

private:
	template <typename F>
	static void find_and_apply_impl(F&& /*functor*/)
	{
	}

	template <typename F, typename Head, typename... Tail>
	static void find_and_apply_impl(F&& functor)
	{
		Head* hack{nullptr};
		if(functor(hack))
			return;
		find_and_apply_impl<F, Tail...>(std::forward<F>(functor));
	}

	template <typename F>
	static void for_each_impl(F&& /*functor*/)
	{
	}

	template <typename F, typename Head, typename... Tail>
	static void for_each_impl(F&& functor)
	{
		Head* hack{nullptr};
		functor(hack);
		for_each_impl<F, Tail...>(std::forward<F>(functor));
	}

public:
	template <typename F>
	static void find_and_apply(F&& functor)
	{
		find_and_apply_impl<F, Types...>(std::forward<F>(functor));
	}

	template <typename F>
	static void for_each(F&& functor)
	{
		for_each_impl<F, Types...>(std::forward<F>(functor));
	}
};

} // namespace rsp::meta