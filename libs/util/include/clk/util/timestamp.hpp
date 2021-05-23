#pragma once

#include <chrono>

namespace clk
{
class timestamp
{
public:
	timestamp() = default;
	timestamp(timestamp const&) = default;
	timestamp(timestamp&&) = default;
	auto operator=(timestamp const&) -> timestamp& = default;
	auto operator=(timestamp&&) -> timestamp& = default;
	~timestamp() = default;

	void update();
	void reset();
	auto is_reset() const -> bool;
	auto operator>(timestamp const& other) const -> bool;
	auto operator<(timestamp const& other) const -> bool;
	auto is_newer_than(timestamp const& other) const -> bool;
	auto is_older_than(timestamp const& other) const -> bool;

private:
	using chrono = std::chrono::steady_clock;
	chrono::time_point _timepoint = chrono::time_point::min();
};

inline void timestamp::update()
{
	_timepoint = chrono::now();
}

inline void timestamp::reset()
{
	_timepoint = chrono::time_point::min();
}

inline auto timestamp::is_reset() const -> bool
{
	return _timepoint == chrono::time_point::min();
}

inline auto timestamp::operator>(timestamp const& other) const -> bool
{
	return this->_timepoint > other._timepoint;
}

inline auto timestamp::operator<(timestamp const& other) const -> bool
{
	return this->_timepoint < other._timepoint;
}

inline auto timestamp::is_newer_than(timestamp const& other) const -> bool
{
	return *this > other;
}

inline auto timestamp::is_older_than(timestamp const& other) const -> bool
{
	return *this < other;
}

} // namespace clk