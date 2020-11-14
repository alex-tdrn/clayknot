#pragma once

#include <memory>
#include <string>

namespace rsp
{
class IData
{
protected:
	std::string name;

public:
	IData() = default;
	IData(IData const&) = delete;
	IData(IData&&) = delete;
	IData& operator=(IData const&) = delete;
	IData& operator=(IData&&) = delete;
	virtual ~IData() = default;

public:
};
} // namespace rsp