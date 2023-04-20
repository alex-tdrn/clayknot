#include "clk/util/profiler.hpp"

#include <algorithm>
#include <numeric>
#include <ratio>

namespace clk
{

profiler::profiler()
{
    set_sample_count(100);
}

auto profiler::is_active() const -> bool
{
    return _active;
}

void profiler::set_active(bool active)
{
    _active = active;
    _subsample_start_time = std::chrono::steady_clock::now();
}

void profiler::set_sample_count(std::size_t count)
{
    _current_sample_index %= count;
    _samples.resize(count, _average_sample);
}

auto profiler::latest_sample_time() const -> std::chrono::steady_clock::time_point const&
{
    return _latest_sample_time;
}

void profiler::record_sample_start()
{
    if(!_active)
        return;
    _subsample_start_time = std::chrono::steady_clock::now();
}

void profiler::record_sample_end()
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

auto profiler::average_sample() const -> std::chrono::nanoseconds
{
    return _average_sample;
}

auto profiler::longest_sample() const -> std::chrono::nanoseconds
{
    return _longest_sample;
}

auto profiler::shortest_sample() const -> std::chrono::nanoseconds
{
    return _shortest_sample;
}

auto profiler::samples() const -> std::pair<std::size_t, std::vector<std::chrono::nanoseconds> const&>
{
    return {_current_sample_index, _samples};
}

} // namespace clk
