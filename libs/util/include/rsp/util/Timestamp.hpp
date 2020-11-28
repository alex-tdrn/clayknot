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

	auto isReset() const -> bool
	{
		return timepoint == chrono::time_point::min();
	}

	auto operator>(Timestamp const& other) const -> bool
	{
		return this->timepoint > other.timepoint;
	}

	auto operator<(Timestamp const& other) const -> bool
	{
		return this->timepoint < other.timepoint;
	}

	auto isNewerThan(Timestamp const& other) const -> bool
	{
		return *this > other;
	}

	auto isOlderThan(Timestamp const& other) const -> bool
	{
		return *this < other;
	}
};

} // namespace rsp