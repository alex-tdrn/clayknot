#pragma once

#include <chrono>

namespace rshp::base
{
class Timestamp final
{
private:
	using chrono = std::chrono::steady_clock;

private:
	chrono::time_point timestamp = chrono::time_point::min();

public:
	Timestamp() = default;
	Timestamp(Timestamp const&) = default;
	Timestamp(Timestamp&&) = default;
	Timestamp& operator=(Timestamp const&) = default;
	Timestamp& operator=(Timestamp&&) = default;
	~Timestamp() = default;

public:
	void update()
	{
		timestamp = chrono::now();
	}

	void reset()
	{
		timestamp = chrono::time_point::min();
	}

	bool isReset() const
	{
		return timestamp == chrono::time_point::min();
	}

	bool isNewerThan(Timestamp const& other) const
	{
		return this->timestamp > other.timestamp;
	}

	bool isOlderThan(Timestamp const& other) const
	{
		return this->timestamp < other.timestamp;
	}
};

} // namespace rshp::base