#pragma once

#include "clk/gui/widgets/data_reader.hpp"
#include "clk/gui/widgets/widget.hpp"
#include "clk/util/bounded.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"
#include "clk/util/time_unit.hpp"
#include "clk/util/type_list.hpp"

#include <any>
#include <exception>
#include <imgui.h>
#include <memory>
#include <range/v3/view.hpp>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

namespace clk::gui
{
class viewer : public widget
{
public:
	viewer() = default;
	viewer(viewer const&) = delete;
	viewer(viewer&&) = delete;
	auto operator=(viewer const&) -> viewer& = delete;
	auto operator=(viewer&&) -> viewer& = delete;
	~viewer() override = default;

	template <typename data_type, typename viewer_implementation>
	static void register_factory();
	template <typename data_type>
	static auto create(data_reader<data_type> data_reader, std::string_view name) -> std::unique_ptr<viewer>;
	static auto create(data_reader<void> data_reader, std::uint64_t data_hash, std::string_view name)
		-> std::unique_ptr<viewer>;

private:
	using factory = std::unique_ptr<viewer> (*)(std::any, std::string_view);
	using nested_data_reader_factory = std::any (*)(data_reader<void>);

	static auto create(std::any data_reader, std::string_view name) -> std::unique_ptr<viewer>;
	static auto factories_map() -> std::unordered_map<std::uint64_t, factory>&;
	static auto nested_data_reader_factories_map() -> std::unordered_map<std::uint64_t, nested_data_reader_factory>&;
};

template <typename data_type>
class viewer_of : public viewer
{
public:
	viewer_of() = default;
	viewer_of(viewer_of const&) = delete;
	viewer_of(viewer_of&&) = delete;
	auto operator=(viewer_of const&) -> viewer_of& = delete;
	auto operator=(viewer_of&&) -> viewer_of& = delete;
	~viewer_of() override = default;

	auto clone() const -> std::unique_ptr<widget> override;
	void copy(widget const& other) override;

	void set_data_reader(data_reader<data_type> data);
	virtual void draw_contents(data_type const& data) const;

private:
	data_reader<data_type> _data;

	void draw_contents() const final;
};

template <typename data_type, typename viewer_implementation>
inline void viewer::register_factory()
{
	auto data_reader_hash = std::type_index(typeid(data_reader<data_type>)).hash_code();
	factories_map()[data_reader_hash] = [](std::any data, std::string_view name) -> std::unique_ptr<viewer> {
		auto viewer = std::make_unique<viewer_implementation>();
		viewer->set_name(name);
		viewer->set_data_reader(std::any_cast<data_reader<data_type>>(data));
		return viewer;
	};

	auto data_hash = std::type_index(typeid(data_type)).hash_code();
	nested_data_reader_factories_map()[data_hash] = [](data_reader<void> type_erased_data_reader) -> std::any {
		return data_reader<data_type>{[nested_reader = std::move(type_erased_data_reader)]() {
			return static_cast<data_type const*>(nested_reader.read());
		}};
	};
}

template <typename data_type>
auto viewer::create(data_reader<data_type> data_reader, std::string_view name) -> std::unique_ptr<viewer>
{
	return viewer::create(std::any(std::move(data_reader)), name);
}

inline auto viewer::create(data_reader<void> data_reader, std::uint64_t data_hash, std::string_view name)
	-> std::unique_ptr<viewer>
{
	if(nested_data_reader_factories_map().count(data_hash) == 0)
		throw std::runtime_error("Cannot create requested viewer");
	return create(nested_data_reader_factories_map()[data_hash](std::move(data_reader)), name);
}

inline auto viewer::create(std::any data_reader, std::string_view name) -> std::unique_ptr<viewer>
{
	auto& factories = factories_map();
	auto hash = data_reader.type().hash_code();
	if(factories.count(hash) == 0)
		throw std::runtime_error("Cannot create requested viewer");
	return factories.at(hash)(std::move(data_reader), name);
}

inline auto viewer::factories_map() -> std::unordered_map<std::uint64_t, factory>&
{
	static auto factories = []() {
		std::unordered_map<std::uint64_t, factory> map;

		using supported_types = meta::type_list<bool, int, float, glm::vec2, glm::vec3, glm::vec4, clk::bounded<int>,
			clk::bounded<float>, clk::bounded<glm::vec2>, clk::bounded<glm::vec3>, clk::bounded<glm::vec4>,
			clk::color_rgb, clk::color_rgba, std::chrono::nanoseconds>;

		supported_types::for_each([&map](auto* dummy) {
			using current_type = std::remove_cv_t<std::remove_pointer_t<decltype(dummy)>>;

			auto data_reader_hash = std::type_index(typeid(data_reader<current_type>)).hash_code();
			assert("Type hash function collision!" && map.count(data_reader_hash) == 0);

			map[data_reader_hash] = [](std::any data, std::string_view name) -> std::unique_ptr<viewer> {
				auto viewer = std::make_unique<viewer_of<current_type>>();
				viewer->set_name(name);
				viewer->set_data_reader(std::any_cast<data_reader<current_type>&&>(std::move(data)));
				return viewer;
			};

			auto data_hash = std::type_index(typeid(current_type)).hash_code();
			nested_data_reader_factories_map()[data_hash] = [](data_reader<void> type_erased_data_reader) -> std::any {
				return data_reader<current_type>{[nested_reader = std::move(type_erased_data_reader)]() {
					return static_cast<current_type const*>(nested_reader.read());
				}};
			};
		});
		return map;
	}();
	return factories;
}

inline auto viewer::nested_data_reader_factories_map() -> std::unordered_map<std::uint64_t, nested_data_reader_factory>&
{
	static std::unordered_map<std::uint64_t, nested_data_reader_factory> map;
	return map;
}

template <typename data_type>
auto viewer_of<data_type>::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<viewer_of<data_type>>();
	clone->copy(*this);
	return clone;
}

template <typename data_type>
void viewer_of<data_type>::copy(widget const& other)
{
	auto const& casted = dynamic_cast<viewer_of<data_type> const&>(other);
	_data = casted._data;
	viewer::copy(other);
}

template <typename data_type>
void viewer_of<data_type>::set_data_reader(data_reader<data_type> data)
{
	_data = std::move(data);
}

template <typename data_type>
void viewer_of<data_type>::draw_contents() const
{
	if(auto const* data = _data.read(); data != nullptr)
	{
		draw_contents(*data);
	}
	else
	{
		ImGui::Text("NO DATA");
	}
}

template <typename data_type>
void viewer_of<data_type>::draw_contents(data_type const& /*data*/) const
{
	ImGui::Text("NO VIEWER IMPLEMENTATION");
}

template <>
inline void viewer_of<bool>::draw_contents(bool const& data) const
{
	ImGui::SameLine();
	if(data) // TODO use icons later when integrating icon fonts
		ImGui::Text("True");
	else
		ImGui::Text("False");
}

template <>
inline void viewer_of<int>::draw_contents(int const& data) const
{
	ImGui::SameLine();
	ImGui::Text("%i", data);
}

template <>
inline void viewer_of<float>::draw_contents(float const& data) const
{
	ImGui::SameLine();
	ImGui::Text("%.3f", data);
}

template <>
inline void viewer_of<glm::vec2>::draw_contents(glm::vec2 const& data) const
{
	ImGui::Text("%.3f, %.3f", data.x, data.y);
}

template <>
inline void viewer_of<glm::vec3>::draw_contents(glm::vec3 const& data) const
{
	ImGui::Text("%.3f, %.3f, %.3f", data.x, data.y, data.z);
}

template <>
inline void viewer_of<glm::vec4>::draw_contents(glm::vec4 const& data) const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", data.x, data.y, data.z, data.w);
}

template <>
inline void viewer_of<clk::bounded<int>>::draw_contents(clk::bounded<int> const& data) const
{
	ImGui::Text("%i (%i - %i)", data.val(), data.min(), data.max());
}

template <>
inline void viewer_of<clk::bounded<float>>::draw_contents(clk::bounded<float> const& data) const
{
	ImGui::Text("%.3f (%.3f - %.3f)", data.val(), data.min(), data.max());
}

template <>
inline void viewer_of<clk::bounded<glm::vec2>>::draw_contents(clk::bounded<glm::vec2> const& data) const
{
	ImGui::Text("%.3f, %.3f", data.val().x, data.val().y);
	if(extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data.min()[0], data.max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data.min()[1], data.max()[1]);
	}
}

template <>
inline void viewer_of<clk::bounded<glm::vec3>>::draw_contents(clk::bounded<glm::vec3> const& data) const
{
	ImGui::Text("%.3f, %.3f, %.3f", data.val().x, data.val().y, data.val().z);
	if(extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data.min()[0], data.max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data.min()[1], data.max()[1]);
		ImGui::Text("Z (%.3f - %.3f)", data.min()[2], data.max()[2]);
	}
}

template <>
inline void viewer_of<clk::bounded<glm::vec4>>::draw_contents(clk::bounded<glm::vec4> const& data) const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", data.val().x, data.val().y, data.val().z, data.val().w);
	if(extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data.min()[0], data.max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data.min()[1], data.max()[1]);
		ImGui::Text("Z (%.3f - %.3f)", data.min()[2], data.max()[2]);
		ImGui::Text("W (%.3f - %.3f)", data.min()[3], data.max()[3]);
	}
}

template <>
inline void viewer_of<clk::color_rgb>::draw_contents(clk::color_rgb const& data) const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f", data.r(), data.g(), data.b());
	if(extended_preferred())
	{
		auto s = available_width();

		ImGui::ColorButton("##", ImVec4(data.r(), data.g(), data.b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", ImVec4(data.r(), data.g(), data.b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void viewer_of<clk::color_rgba>::draw_contents(clk::color_rgba const& data) const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f, A:%.3f", data.r(), data.g(), data.b(), data.a());
	if(extended_preferred())
	{
		auto s = available_width();

		ImGui::ColorButton("##", data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void viewer_of<std::chrono::nanoseconds>::draw_contents(std::chrono::nanoseconds const& data) const
{
	auto time_units = time_unit::decompose(data);
	if(extended_preferred())
	{
		for(auto const& unit : time_units)
		{
			if(unit.value != 0)
			{
				ImGui::Text(("%i" + unit.suffix).c_str(), unit.value);
				ImGui::SameLine();
			}
		}
	}
	else
	{
		const std::size_t max_units_to_draw = 2;
		auto non_empty_units = time_units | ranges::views::drop_while([](auto unit) {
			return unit.value <= 0;
		});

		for(auto const& unit : non_empty_units | ranges::views::take(max_units_to_draw))
		{
			ImGui::Text(("%i" + unit.suffix).c_str(), unit.value);
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}
}

} // namespace clk::gui
