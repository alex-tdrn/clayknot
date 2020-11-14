#pragma once

namespace rsp
{
class Sentinel
{
public:
	Sentinel() = default;
	Sentinel(Sentinel const&) = delete;
	Sentinel(Sentinel&&) = delete;
	Sentinel& operator=(Sentinel const&) = delete;
	Sentinel& operator=(Sentinel&&) = delete;
	virtual ~Sentinel() = default;
};
} // namespace rsp