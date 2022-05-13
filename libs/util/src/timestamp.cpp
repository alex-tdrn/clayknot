#include "clk/util/timestamp.hpp"

namespace clk
{

void timestamp::update()
{
	_timepoint = chrono::now();
}

void timestamp::reset()
{
	_timepoint = chrono::time_point::min();
}

auto timestamp::is_reset() const -> bool
{
	return _timepoint == chrono::time_point::min();
}

auto timestamp::operator>(timestamp const& other) const -> bool
{
	return this->_timepoint > other._timepoint;
}

auto timestamp::operator<(timestamp const& other) const -> bool
{
	return this->_timepoint < other._timepoint;
}

auto timestamp::is_newer_than(timestamp const& other) const -> bool
{
	return *this > other;
}

auto timestamp::is_older_than(timestamp const& other) const -> bool
{
	return *this < other;
}

auto timestamp::time_point() const -> std::chrono::steady_clock::time_point
{
	return _timepoint;
}

} // namespace clk