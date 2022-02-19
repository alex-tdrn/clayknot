#pragma once

#include <algorithm>
#include <chrono>
#include <numeric>
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

inline profiler::profiler()
{
	set_sample_count(100);
}

inline auto profiler::is_active() const -> bool
{
	return _active;
}

inline void profiler::set_active(bool active)
{
	_active = active;
	_subsample_start_time = std::chrono::steady_clock::now();
}

inline void profiler::set_sample_count(std::size_t count)
{
	_current_sample_index %= count;
	_samples.resize(count, _average_sample);
}

inline auto profiler::latest_sample_time() const -> std::chrono::steady_clock::time_point const&
{
	return _latest_sample_time;
}

inline void profiler::record_sample_start()
{
	if(!_active)
		return;
	_subsample_start_time = std::chrono::steady_clock::now();
}

inline void profiler::record_sample_end()
{
	if(!_active)
		return;
	_latest_sample_time = std::chrono::steady_clock::now();

	_samples[_current_sample_index] = _latest_sample_time - _subsample_start_time;

	_current_sample_index = (_current_sample_index + 1) % _samples.size();

	_average_sample = std::accumulate(_samples.begin(), _samples.end(), 0ns) / _samples.size();
	{
		auto [min_iterator, max_iterator] = std::minmax_element(_samples.begin(), _samples.end());
		_shortest_sample = *min_iterator;
		_longest_sample = *max_iterator;
	}
}

inline auto profiler::average_sample() const -> std::chrono::nanoseconds
{
	return _average_sample;
}

inline auto profiler::longest_sample() const -> std::chrono::nanoseconds
{
	return _longest_sample;
}

inline auto profiler::shortest_sample() const -> std::chrono::nanoseconds
{
	return _shortest_sample;
}

inline auto profiler::samples() const -> std::pair<std::size_t, std::vector<std::chrono::nanoseconds> const&>
{
	return {_current_sample_index, _samples};
}

} // namespace clk