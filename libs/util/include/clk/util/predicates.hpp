#pragma once

#include <memory>
#include <type_traits>

namespace clk
{
template <typename T>
static constexpr auto is_equal_to(T&& value)
{
	return [a = std::forward<T>(value)](auto b) {
		return a == b;
	};
}

template <typename T>
static constexpr auto is_not_equal_to(T&& value)
{
	return [a = std::forward<T>(value)](auto b) {
		return a != b;
	};
}

} // namespace clk