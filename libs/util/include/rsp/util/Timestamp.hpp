#pragma once

#include <chrono>

namespace rsp
{
class Timestamp
{
private:
	using chrono = std::chrono::steady_clock;

private:
	chrono::time_point timepoint = chrono::time_point::min();

public:
	void update()
	{
		timepoint = chrono::now();
	}

	void reset()
	{
		timepoint = chrono::time_point::min();
	}

	bool isReset() const
	{
		return timepoint == chrono::time_point::min();
	}

	bool operator>(Timestamp const& other) const
	{
		return this->timepoint > other.timepoint;
	}

	bool operator<(Timestamp const& other) const
	{
		return this->timepoint < other.timepoint;
	}

	bool isNewerThan(Timestamp const& other) const
	{
		return *this > other;
	}

	bool isOlderThan(Timestamp const& other) const
	{
		return *this < other;
	}
};

} // namespace rsp