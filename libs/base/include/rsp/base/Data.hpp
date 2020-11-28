#pragma once

#include <memory>
#include <string>

namespace rsp
{
class IData
{
private:
	std::string name;

public:
	IData() = default;
	IData(IData const&) = delete;
	IData(IData&&) = delete;
	auto operator=(IData const&) -> IData& = delete;
	auto operator=(IData&&) -> IData& = delete;
	virtual ~IData() = default;
};
} // namespace rsp