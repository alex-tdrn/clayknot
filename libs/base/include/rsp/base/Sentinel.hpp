#pragma once

namespace rsp
{
class Sentinel
{
public:
	Sentinel() = default;
	Sentinel(Sentinel const&) = delete;
	Sentinel(Sentinel&&) = delete;
	auto operator=(Sentinel const&) -> Sentinel& = delete;
	auto operator=(Sentinel&&) -> Sentinel& = delete;
	virtual ~Sentinel() = default;
};
} // namespace rsp