#pragma once

#include <chrono>
#include <cstddef>
#include <utility>
#include <vector>

namespace clk
{
using namespace std::chrono_literals;

class profiler
{
public:
	profiler();
	profiler(profiler const&) = default;
	profiler(profiler&&) = default;
	auto operator=(profiler const&) -> profiler& = default;
	auto operator=(profiler&&) -> profiler& = default;
	~profiler() = default;

	auto is_active() const -> bool;
	void set_active(bool active);

	void set_sample_count(std::size_t count);
	void record_sample_start();
	void record_sample_end();

	auto latest_sample_time() const -> std::chrono::steady_clock::time_point const&;
	auto average_sample() const -> std::chrono::nanoseconds;
	auto longest_sample() const -> std::chrono::nanoseconds;
	auto shortest_sample() const -> std::chrono::nanoseconds;
	auto samples() const -> std::pair<std::size_t, std::vector<std::chrono::nanoseconds> const&>;

private:
	bool _active = true;
	std::chrono::steady_clock::time_point _subsample_start_time = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point _latest_sample_time = std::chrono::steady_clock::now();
	std::size_t _current_sample_index = 0;
	std::vector<std::chrono::nanoseconds> _samples;
	std::chrono::nanoseconds _average_sample = 0ns;
	std::chrono::nanoseconds _longest_sample = 0ns;
	std::chrono::nanoseconds _shortest_sample = 0ns;
};

} // namespace clk