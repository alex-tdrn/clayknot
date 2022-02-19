#pragma once

#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/widget_factory.hpp"
#include "clk/gui/widgets/widget_tree.hpp"
#include "clk/util/bounded.hpp"
#include "clk/util/profiler.hpp"

#include <chrono>
#include <imgui.h>
#include <implot.h>
#include <range/v3/functional/not_fn.hpp>

namespace clk::gui
{

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
	void draw_helper(clk::profiler const& profiler) const;
};

inline profiler_editor::profiler_editor(std::shared_ptr<widget_factory const> factory, std::string_view name)
	: editor_of<clk::profiler>(std::move(factory), name)
{
	auto const& f = get_widget_factory();
	auto& plot_settings = settings().get_subtree("Plot");
	plot_settings.add(f->create(_plot_height, "Height"));
	plot_settings.add(f->create(_plot_width, "Width"));
	plot_settings.add(f->create(_plot_alpha, "Fill opacity"));
}

inline auto profiler_editor::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<profiler_editor>(this->get_widget_factory(), this->name());
	clone->copy(*this);
	return clone;
}

inline void profiler_editor::copy(widget const& other)
{
	auto const& casted = dynamic_cast<profiler_editor const&>(other);
	_plot_height = casted._plot_height;
	editor_of<clk::profiler>::copy(other);
}

inline void profiler_editor::set_plot_height(float height)
{
	_plot_height = height;
}

inline auto profiler_editor::draw_contents(clk::profiler& profiler) const -> bool
{
	bool modified = false;

	if(auto active = profiler.is_active(); ImGui::Checkbox("Active", &active))
	{
		profiler.set_active(active);
		modified = true;
	}
	ImGui::SameLine();

	ImGui::Text("Sample count");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);
	if(auto sample_count = static_cast<int>(profiler.samples().second.size());
		ImGui::DragInt("###Sample count", &sample_count, 1.0f, 2))
	{
		profiler.set_sample_count(static_cast<std::size_t>(std::max(sample_count, 2)));
		modified = true;
	}

	draw_helper<std::nano>(profiler);

	return modified;
}

template <typename Ratio>
void profiler_editor::draw_helper(clk::profiler const& profiler) const
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
			70.0f, 80.0f, 90.0f, 100.0f, 125.0f, 150.0f, 175.0f, 200.0f, 225.0f, 250.0f, 300.0f, 350.0f, 400.0f, 450.0f,
			500.0f, 750.0f, 1000.0f})
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

	ImPlot::SetNextPlotLimitsY(0, upper_limit, ImGuiCond_Always);
	ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, _plot_alpha.val());
	ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0, 0, 0, 0));
	if(ImPlot::BeginPlot(name().data(), nullptr, unit_postfix_long.c_str(), ImVec2(_plot_width, _plot_height),
		   ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMousePos |
			   ImPlotFlags_AntiAliased | ImPlotFlags_NoChild,
		   ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations))
	{
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

} // namespace clk::gui