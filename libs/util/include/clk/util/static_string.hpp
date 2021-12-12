#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace clk
{
template <std::size_t N>
struct static_string
{
	constexpr static_string(const char (&s)[N + 1])
	{
		std::copy(s, s + N, _string);
	}

	char _string[N + 1] = {};
};

template <std::size_t N>
static_string(const char (&s)[N]) -> static_string<N - 1>;

} // namespace clk