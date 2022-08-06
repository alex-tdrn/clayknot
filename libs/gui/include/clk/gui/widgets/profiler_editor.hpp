#pragma once

#include "clk/gui/widgets/editor.hpp"
#include "clk/util/bounded.hpp"
#include "clk/util/profiler.hpp"

#define IMPLOT_DISABLE_OBSOLETE_FUNCTIONS

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <imgui.h>
#include <implot.h>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ratio>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace clk::gui
{
class widget;
class widget_factory;

class profiler_editor final : public editor_of<clk::profiler>
{
public:
	profiler_editor(std::shared_ptr<widget_factory const> factory, std::string_view name);
	profiler_editor() = delete;
	profiler_editor(profiler_editor const&) = delete;
	profiler_editor(profiler_editor&&) = delete;
	auto operator=(profiler_editor const&) -> profiler_editor& = delete;
	auto operator=(profiler_editor&&) -> profiler_editor& = delete;
	~profiler_editor() override = default;

	auto clone() const -> std::unique_ptr<widget> override;
	void copy(widget const& other) override;

	void set_plot_height(float height);
	auto draw_contents(clk::profiler& profiler) const -> bool final;

private:
	float _plot_width = 0;
	float _plot_height = 200;
	clk::bounded<float> _plot_alpha{0.25f, 0.0f, 1.0f};
	mutable std::vector<float> _samples;
	mutable std::chrono::steady_clock::time_point _last_profiler_sampling_time =
		std::chrono::steady_clock::time_point::min();
	mutable std::chrono::steady_clock::time_point _high_water_mark_change_time = std::chrono::steady_clock::now();
	mutable std::chrono::nanoseconds _high_water_mark = 0ns;
	mutable std::chrono::nanoseconds _high_water_mark_reset_timeout = 1s;

	template <typename Ratio>
	void draw_helper(clk::profiler const& profiler) const
	{
		std::string unit_postfix_short; // TODO use fmt someday
		std::string unit_postfix_long; // TODO use fmt someday

		auto timescale_too_small = [&]() {
			auto previous_longest_at_this_scale =
				std::chrono::duration_cast<std::chrono::duration<uint64_t, Ratio>>(_high_water_mark).count();
			auto current_longest_at_this_scale =
				std::chrono::duration_cast<std::chrono::duration<uint64_t, Ratio>>(profiler.longest_sample()).count();

			int constexpr threshold = 1'000;
			return previous_longest_at_this_scale >= threshold || current_longest_at_this_scale >= threshold;
		};

		if constexpr(std::is_same_v<Ratio, std::nano>)
		{
			if(timescale_too_small())
			{
				return draw_helper<std::micro>(profiler);
			}
			else
			{
				unit_postfix_short = "ns";
				unit_postfix_long = "nanoseconds";
			}
		}
		else if constexpr(std::is_same_v<Ratio, std::micro>)
		{
			if(timescale_too_small())
			{
				return draw_helper<std::milli>(profiler);
			}
			else
			{
				unit_postfix_short = "us";
				unit_postfix_long = "microseconds";
			}
		}
		else if constexpr(std::is_same_v<Ratio, std::milli>)
		{
			if(timescale_too_small())
			{
				return draw_helper<std::ratio<1, 1>>(profiler);
			}
			else
			{
				unit_postfix_short = "ms";
				unit_postfix_long = "milliseconds";
			}
		}
		else if constexpr(std::is_same_v<Ratio, std::ratio<1, 1>>)
		{
			std::ignore = timescale_too_small;
			unit_postfix_short = "s";
			unit_postfix_long = "seconds";
		}

		auto [offset, samples] = profiler.samples();

		double upper_limit =
			std::chrono::duration_cast<std::chrono::duration<double, Ratio>>(profiler.longest_sample()).count();

		{
			auto now = std::chrono::steady_clock::now();
			if(profiler.longest_sample() < _high_water_mark)
			{
				if((now - _high_water_mark_change_time) > _high_water_mark_reset_timeout)
				{
					_high_water_mark_change_time = now;
					_high_water_mark = profiler.longest_sample();
				}
				else
				{
					upper_limit =
						std::chrono::duration_cast<std::chrono::duration<double, Ratio>>(_high_water_mark).count();
				}
			}
			else
			{
				_high_water_mark_change_time = now;
				_high_water_mark = profiler.longest_sample();
			}
		}

		for(auto step : {1.5f, 2.0f, 2.5f, 5.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 50.0f, 60.0f,
				70.0f, 80.0f, 90.0f, 100.0f, 125.0f, 150.0f, 175.0f, 200.0f, 225.0f, 250.0f, 300.0f, 350.0f, 400.0f,
				450.0f, 500.0f, 750.0f, 1000.0f})
		{
			if(upper_limit < step)
			{
				upper_limit = step;
				break;
			}
		}

		if(_last_profiler_sampling_time != profiler.latest_sample_time())
		{
			_samples.clear();
			_samples.reserve(samples.size());
			auto cast_sample = [](auto& sample) {
				auto casted_sample = std::chrono::duration_cast<std::chrono::duration<float, Ratio>>(sample);
				return casted_sample.count();
			};

			std::transform(
				samples.begin() + static_cast<int>(offset), samples.end(), std::back_inserter(_samples), cast_sample);
			std::transform(
				samples.begin(), samples.begin() + static_cast<int>(offset), std::back_inserter(_samples), cast_sample);

			_last_profiler_sampling_time = profiler.latest_sample_time();
		}

		ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, _plot_alpha.val());
		ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0, 0, 0, 0));
		if(ImPlot::BeginPlot(name().data(), ImVec2(_plot_width, _plot_height),
			   ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText |
				   ImPlotFlags_AntiAliased | ImPlotFlags_NoChild))
		{
			ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
			ImPlot::SetupAxisFormat(ImAxis_Y1, unit_postfix_long.c_str());
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, upper_limit, ImGuiCond_Always);
			ImPlot::PlotShaded("sample", _samples.data(), static_cast<int>(_samples.size()));
			ImPlot::PlotLine("sample", _samples.data(), static_cast<int>(_samples.size()));
			ImPlot::EndPlot();
		}
		ImPlot::PopStyleColor();
		ImPlot::PopStyleVar();

		if(ImGui::BeginTable("###sample_table", 6))
		{
			auto shortest_sample_float =
				std::chrono::duration_cast<std::chrono::duration<double, Ratio>>(profiler.shortest_sample());

			ImGui::TableNextColumn();
			ImGui::Text("Best");
			ImGui::TableNextColumn();
			ImGui::Text("%.2f %s", shortest_sample_float.count(), unit_postfix_short.c_str());

			auto average_sample_float =
				std::chrono::duration_cast<std::chrono::duration<double, Ratio>>(profiler.average_sample());

			ImGui::TableNextColumn();
			ImGui::Text("Average");
			ImGui::TableNextColumn();
			ImGui::Text("%.2f %s", average_sample_float.count(), unit_postfix_short.c_str());

			auto longest_sample_float =
				std::chrono::duration_cast<std::chrono::duration<double, Ratio>>(profiler.longest_sample());

			ImGui::TableNextColumn();
			ImGui::Text("Worst");
			ImGui::TableNextColumn();
			ImGui::Text("%.2f %s", longest_sample_float.count(), unit_postfix_short.c_str());

			ImGui::EndTable();
		}
	}
};

} // namespace clk::gui