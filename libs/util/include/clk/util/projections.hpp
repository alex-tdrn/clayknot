#pragma once
namespace clk
{
static constexpr auto underlying()
{
	return [](auto const& wrapper) {
		return wrapper.get();
	};
}
} // namespace clk