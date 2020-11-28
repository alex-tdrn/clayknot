#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace rsp
{
class Observable
{
private:
	mutable std::unordered_map<int, std::vector<bool*>> observerFlags;

protected:
	void notifyObserverFlags(int flagType) const
	{
		for(auto* const flag : observerFlags[flagType])
			*flag = true;
	}

public:
	void registerObserverFlag(int flagType, bool* flag)
	{
		observerFlags[flagType].push_back(flag);
	}

	void deregisterObserverFlag(bool* /*flag*/)
	{
		for(auto& observerFlagsCategory : observerFlags)
		{
		}
	}

	void deregisterObserverFlag(int flagType, bool* flag)
	{
	}
};
} // namespace rsp