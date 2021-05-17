#pragma once

namespace clk
{
class sentinel
{
public:
	sentinel() = default;
	sentinel(sentinel const&) = delete;
	sentinel(sentinel&&) = delete;
	auto operator=(sentinel const&) -> sentinel& = delete;
	auto operator=(sentinel&&) -> sentinel& = delete;
	virtual ~sentinel() = default;
};
} // namespace clk