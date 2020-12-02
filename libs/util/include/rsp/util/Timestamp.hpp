#pragma once

#include <chrono>

namespace rsp
{
class Timestamp
{
public:
	Timestamp() = default;
	Timestamp(Timestamp const&) = default;
	Timestamp(Timestamp&&) = default;
	auto operator=(Timestamp const&) -> Timestamp& = default;
	auto operator=(Timestamp&&) -> Timestamp& = default;
	~Timestamp() = default;

	void update();
	void reset();
	auto isReset() const -> bool;
	auto operator>(Timestamp const& other) const -> bool;
	auto operator<(Timestamp const& other) const -> bool;
	auto isNewerThan(Timestamp const& other) const -> bool;
	auto isOlderThan(Timestamp const& other) const -> bool;

private:
	using chrono = std::chrono::steady_clock;
	chrono::time_point timepoint = chrono::time_point::min();
};

inline void Timestamp::update()
{
	timepoint = chrono::now();
}

inline void Timestamp::reset()
{
	timepoint = chrono::time_point::min();
}

inline auto Timestamp::isReset() const -> bool
{
	return timepoint == chrono::time_point::min();
}

inline auto Timestamp::operator>(Timestamp const& other) const -> bool
{
	return this->timepoint > other.timepoint;
}

inline auto Timestamp::operator<(Timestamp const& other) const -> bool
{
	return this->timepoint < other.timepoint;
}

inline auto Timestamp::isNewerThan(Timestamp const& other) const -> bool
{
	return *this > other;
}

inline auto Timestamp::isOlderThan(Timestamp const& other) const -> bool
{
	return *this < other;
}

} // namespace rsp