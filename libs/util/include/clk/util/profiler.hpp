#pragma once

#include <chrono>
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

	void set_sample_count(std::size_t count);
	void set_sampling_interval(std::chrono::nanoseconds interval);
	auto last_sampling_time() const -> std::chrono::high_resolution_clock::time_point const&;
	void record_frame();
	auto average_frametime() const -> std::chrono::nanoseconds;
	auto longest_frametime() const -> std::chrono::nanoseconds;
	auto shortest_frametime() const -> std::chrono::nanoseconds;
	auto all_frametimes() const -> std::pair<std::size_t, std::vector<std::chrono::nanoseconds> const&>;

private:
	std::size_t _current_sample_index = 0;
	std::size_t _sample_count = 0;
	std::chrono::high_resolution_clock::time_point _last_frame = std::chrono::high_resolution_clock::now();
	std::chrono::nanoseconds _sampling_interval = 0ns;
	std::chrono::high_resolution_clock::time_point _last_sampling_time = std::chrono::high_resolution_clock::now();
	std::chrono::nanoseconds _current_sample_sum = 0ns;
	std::size_t _current_sample_subsamples_count = 0;
	std::vector<std::chrono::nanoseconds> _all_frametimes;
	std::chrono::nanoseconds _average_frametime = 0ns;
	std::chrono::nanoseconds _longest_frametime = 0ns;
	std::chrono::nanoseconds _shortest_frametime = 0ns;
	bool _first_run = true;
};

inline profiler::profiler()
{
	set_sample_count(100);
	set_sampling_interval(100ms);
}

inline void profiler::set_sample_count(std::size_t count)
{
	_first_run = true;
	_sample_count = count;
	_current_sample_index = 0;
	_all_frametimes.resize(_sample_count, 0ns);
}

inline void profiler::set_sampling_interval(std::chrono::nanoseconds interval)
{
	_sampling_interval = interval;
}

inline auto profiler::last_sampling_time() const -> std::chrono::high_resolution_clock::time_point const&
{
	return _last_sampling_time;
}

inline void profiler::record_frame()
{
	auto current_frame = std::chrono::high_resolution_clock::now();

	_current_sample_sum += std::chrono::duration_cast<std::chrono::nanoseconds>(current_frame - _last_frame);
	_current_sample_subsamples_count++;

	if((current_frame - _last_sampling_time) > _sampling_interval)
	{
		_all_frametimes[_current_sample_index] = _current_sample_sum / _current_sample_subsamples_count;

		_current_sample_index = (_current_sample_index + 1) % _sample_count;
		if(_current_sample_index == 0)
			_first_run = false;

		_average_frametime = 0ns;
		_longest_frametime = 0ns;
		_shortest_frametime = std::chrono::nanoseconds::max();
		for(auto const& frametime : _all_frametimes)
		{
			_average_frametime += frametime;
			_longest_frametime = std::max(_longest_frametime, frametime);
			if(!_first_run || (_first_run && frametime != 0ns))
				_shortest_frametime = std::min(_shortest_frametime, frametime);
		}

		if(_first_run)
			_average_frametime /= static_cast<unsigned int>(_current_sample_index);
		else
			_average_frametime /= static_cast<unsigned int>(_all_frametimes.size());

		_current_sample_sum = 0ns;
		_current_sample_subsamples_count = 0;

		_last_sampling_time = current_frame;
	}
	_last_frame = current_frame;
}

inline auto profiler::average_frametime() const -> std::chrono::nanoseconds
{
	return _average_frametime;
}

inline auto profiler::longest_frametime() const -> std::chrono::nanoseconds
{
	return _longest_frametime;
}

inline auto profiler::shortest_frametime() const -> std::chrono::nanoseconds
{
	return _shortest_frametime;
}

inline auto profiler::all_frametimes() const -> std::pair<std::size_t, std::vector<std::chrono::nanoseconds> const&>
{
	return {_current_sample_index, _all_frametimes};
}

} // namespace clk