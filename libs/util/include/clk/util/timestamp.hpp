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
	auto time_point() const -> std::chrono::steady_clock::time_point;

private:
	using chrono = std::chrono::steady_clock;
	chrono::time_point _timepoint = chrono::time_point::min();
};

} // namespace clk