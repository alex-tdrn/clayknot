#pragma once

#include "clk/gui/widgets/viewer.hpp"
#include "clk/util/profiler.hpp"

#include <implot.h>

namespace clk::gui
{
class profiler_viewer final : public viewer_of<clk::profiler>
{
public:
	profiler_viewer() = delete;
	profiler_viewer(clk::profiler const* data, std::string_view data_name);
	profiler_viewer(profiler_viewer const&) = delete;
	profiler_viewer(profiler_viewer&&) = delete;
	auto operator=(profiler_viewer const&) -> profiler_viewer& = delete;
	auto operator=(profiler_viewer&&) -> profiler_viewer& = delete;
	~profiler_viewer() override = default;

	void set_plot_height(float height);
	auto clone() const -> std::unique_ptr<clk::gui::widget> final;
	void draw_contents() const final;

private:
	float _plot_height = 100;
	mutable std::vector<std::uint32_t> _samples;
	mutable std::chrono::high_resolution_clock::time_point _last_profiler_sampling_time =
		std::chrono::high_resolution_clock::time_point::min();

	template <typename T, typename Ratio>
	void draw_contents(std::chrono::duration<T, Ratio> average_frametime) const;
};

inline profiler_viewer::profiler_viewer(clk::profiler const* data, std::string_view data_name)
	: viewer_of<clk::profiler>(data, data_name)
{
}

inline auto profiler_viewer::clone() const -> std::unique_ptr<clk::gui::widget>
{
	return std::make_unique<profiler_viewer>(data(), data_name());
}

inline void profiler_viewer::set_plot_height(float height)
{
	_plot_height = height;
}

inline void profiler_viewer::draw_contents() const
{
	draw_contents(data()->average_frametime());
}

template <typename T, typename Ratio>
void profiler_viewer::draw_contents(std::chrono::duration<T, Ratio> average_frametime) const
{
	static int const threshold = 1'000;
	std::string unit_postfix_short; // TODO use fmt someday
	std::string unit_postfix_long; // TODO use fmt someday
	if constexpr(std::is_same_v<Ratio, std::nano>)
	{
		if(average_frametime.count() < threshold)
		{
			unit_postfix_short = "ns";
			unit_postfix_long = "nanoseconds";
		}
		else
		{
			return draw_contents(std::chrono::duration_cast<std::chrono::microseconds>(average_frametime));
		}
	}
	else if constexpr(std::is_same_v<Ratio, std::micro>)
	{
		if(average_frametime.count() < threshold)
		{
			unit_postfix_short = "us";
			unit_postfix_long = "microseconds";
		}
		else
		{
			return draw_contents(std::chrono::duration_cast<std::chrono::milliseconds>(average_frametime));
		}
	}
	else if constexpr(std::is_same_v<Ratio, std::milli>)
	{
		if(average_frametime.count() < threshold)
		{
			unit_postfix_short = "ms";
			unit_postfix_long = "milliseconds";
		}
		else
		{
			return draw_contents(std::chrono::duration_cast<std::chrono::seconds>(average_frametime));
		}
	}
	else if constexpr(std::is_same_v<Ratio, std::ratio<1, 1>>)
	{
		unit_postfix_short = "s";
		unit_postfix_long = "seconds";
	}

	const auto& profiler = *data();
	auto longest_frametime = std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(profiler.longest_frametime());
	if(ImGui::BeginTable("###frametime_table", 3))
	{
		auto shortest_frametime =
			std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(profiler.shortest_frametime());

		ImGui::TableNextColumn();
		ImGui::Text("Best");
		ImGui::TableNextColumn();
		ImGui::Text("%s %s", std::to_string(shortest_frametime.count()).c_str(), unit_postfix_short.c_str());
		ImGui::TableNextColumn();
		int best_fps =
			1.0f /
			std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(shortest_frametime).count();
		ImGui::Text("%i fps", best_fps);

		ImGui::TableNextColumn();
		ImGui::Text("Average");
		ImGui::TableNextColumn();
		ImGui::Text("%s %s", std::to_string(average_frametime.count()).c_str(), unit_postfix_short.c_str());
		ImGui::TableNextColumn();
		int average_fps =
			1.0f /
			std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(average_frametime).count();
		ImGui::Text("%i fps", average_fps);

		ImGui::TableNextColumn();
		ImGui::Text("Worst");
		ImGui::TableNextColumn();
		ImGui::Text("%s %s", std::to_string(longest_frametime.count()).c_str(), unit_postfix_short.c_str());
		ImGui::TableNextColumn();
		int worst_fps =
			1.0f /
			std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(longest_frametime).count();
		ImGui::Text("%i fps", worst_fps);

		ImGui::EndTable();
	}

	auto [offset, frametimes] = profiler.all_frametimes();

	auto upper_limit = static_cast<std::uint32_t>(longest_frametime.count());

	for(auto step : {10u, 25u, 50u, 100u, 250u, 500u, 1'000u, 1'500u, 2'000u, 2'500u})
	{
		if(upper_limit < step)
		{
			upper_limit = step;
			break;
		}
	}

	if(_last_profiler_sampling_time != profiler.last_sampling_time())
	{
		_samples.clear();
		_samples.reserve(frametimes.size());
		auto cast_frametime = [](auto& frametime) {
			auto casted_frametime = std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(frametime);
			return static_cast<std::uint32_t>(casted_frametime.count());
		};

		std::transform(frametimes.begin() + static_cast<int>(offset), frametimes.end(), std::back_inserter(_samples),
			cast_frametime);
		std::transform(frametimes.begin(), frametimes.begin() + static_cast<int>(offset), std::back_inserter(_samples),
			cast_frametime);

		_last_profiler_sampling_time = profiler.last_sampling_time();
	}

	ImPlot::SetNextPlotLimitsY(0, static_cast<double>(upper_limit), ImGuiCond_Always);
	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
	ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0, 0, 0, 0));
	if(ImPlot::BeginPlot("Frametimes", nullptr, unit_postfix_long.c_str(), ImVec2(-1, -1),
		   ImPlotFlags_CanvasOnly | ImPlotFlags_AntiAliased | ImPlotFlags_NoChild,
		   ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations))
	{
		ImPlot::PlotShaded("Frametime", _samples.data(), static_cast<int>(_samples.size()));
		ImPlot::PlotLine("Frametime", _samples.data(), static_cast<int>(_samples.size()));
		ImPlot::EndPlot();
	}
	ImPlot::PopStyleColor();
	ImPlot::PopStyleVar();
}

} // namespace clk::gui