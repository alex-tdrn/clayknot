#pragma once

#include "clk/gui/widgets/widget.hpp"
#include "clk/util/bounded.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"
#include "clk/util/time_unit.hpp"
#include "clk/util/type_list.hpp"

#include <imgui.h>
#include <range/v3/view.hpp>
#include <typeindex>
#include <unordered_map>

namespace clk::gui
{
class viewer : public widget
{
public:
	using factory = std::unique_ptr<viewer> (*)(void const*, std::string const&);

	viewer() = delete;
	explicit viewer(std::string const& data_name);
	viewer(viewer const&) = delete;
	viewer(viewer&&) = delete;
	auto operator=(viewer const&) -> viewer& = delete;
	auto operator=(viewer&&) -> viewer& = delete;
	~viewer() override = default;

	template <typename data_type, typename viewer_implementation>
	static void register_factory();
	template <typename data_type>
	static auto create(data_type const* data, std::string const& data_name) -> std::unique_ptr<viewer>;
	static auto create(std::size_t type_hash, void const* data, std::string const& data_name)
		-> std::unique_ptr<viewer>;
	virtual void update_data_pointer(void const* data) = 0;

private:
	static auto factories_map() -> std::unordered_map<std::uint64_t, factory>&;
};

template <typename data_type>
class viewer_of : public viewer
{
public:
	viewer_of() = delete;
	viewer_of(data_type const* data, std::string const& data_name);
	viewer_of(viewer_of const&) = delete;
	viewer_of(viewer_of&&) = delete;
	auto operator=(viewer_of const&) -> viewer_of& = delete;
	auto operator=(viewer_of&&) -> viewer_of& = delete;
	~viewer_of() override = default;

	auto clone() const -> std::unique_ptr<widget> override;
	void update_data_pointer(void const* data) final;
	auto data_available() const noexcept -> bool final;
	void draw_contents() const override;

protected:
	auto data() const -> data_type const*;

private:
	data_type const* _data;
};

template <typename data_type, typename viewer_implementation>
inline void viewer::register_factory()
{
	factories_map()[std::type_index(typeid(data_type)).hash_code()] =
		[](void const* data, std::string const& data_name) -> std::unique_ptr<viewer> {
		return std::make_unique<viewer_implementation>(static_cast<data_type const*>(data), data_name);
	};
}

inline auto viewer::create(std::size_t type_hash, void const* data, std::string const& data_name)
	-> std::unique_ptr<viewer>
{
	auto& factories = factories_map();
	if(factories.count(type_hash) == 0)
		return std::make_unique<viewer_of<void>>(data, data_name);
	return factories.at(type_hash)(data, data_name);
}

template <typename data_type>
auto viewer::create(data_type const* data, std::string const& data_name) -> std::unique_ptr<viewer>
{
	return viewer::create(std::type_index(typeid(data_type)).hash_code(), data, data_name);
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

			std::size_t hash = std::type_index(typeid(current_type)).hash_code();
			assert("Type hash function collision!" && map.count(hash) == 0);

			map[hash] = [](void const* data, std::string const& dataName) -> std::unique_ptr<viewer> {
				return std::make_unique<viewer_of<current_type>>(static_cast<current_type const*>(data), dataName);
			};
		});
		return map;
	}();
	return factories;
}

inline viewer::viewer(std::string const& data_name) : widget(data_name)
{
}

template <typename data_type>
viewer_of<data_type>::viewer_of(data_type const* data, std::string const& data_name) : viewer(data_name), _data(data)
{
}

template <typename data_type>
auto viewer_of<data_type>::clone() const -> std::unique_ptr<widget>
{
	auto cloned_editor = std::make_unique<viewer_of<data_type>>(_data, data_name());
	return cloned_editor;
}

template <typename data_type>
void viewer_of<data_type>::update_data_pointer(void const* data)
{
	_data = static_cast<data_type const*>(data);
}

template <typename data_type>
auto viewer_of<data_type>::data_available() const noexcept -> bool
{
	return _data != nullptr;
}

template <typename data_type>
void viewer_of<data_type>::draw_contents() const
{
	ImGui::Text("NO VIEWER IMPLEMENTATION");
}

template <typename data_type>
auto viewer_of<data_type>::data() const -> data_type const*
{
	return _data;
}

template <>
inline void viewer_of<bool>::draw_contents() const
{
	ImGui::SameLine();
	if(*_data)
		ImGui::Text("On");
	else
		ImGui::Text("Off");
}

template <>
inline void viewer_of<int>::draw_contents() const
{
	ImGui::SameLine();
	ImGui::Text("%i", *_data);
}

template <>
inline void viewer_of<float>::draw_contents() const
{
	ImGui::SameLine();
	ImGui::Text("%.3f", *_data);
}

template <>
inline void viewer_of<glm::vec2>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f", _data->x, _data->y);
}

template <>
inline void viewer_of<glm::vec3>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f, %.3f", _data->x, _data->y, _data->z);
}

template <>
inline void viewer_of<glm::vec4>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", _data->x, _data->y, _data->z, _data->w);
}

template <>
inline void viewer_of<clk::bounded<int>>::draw_contents() const
{
	ImGui::Text("%i (%i - %i)", _data->val(), _data->min(), _data->max());
}

template <>
inline void viewer_of<clk::bounded<float>>::draw_contents() const
{
	ImGui::Text("%.3f (%.3f - %.3f)", _data->val(), _data->min(), _data->max());
}

template <>
inline void viewer_of<clk::bounded<glm::vec2>>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f", _data->val().x, _data->val().y);
	if(extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", _data->min()[0], _data->max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", _data->min()[1], _data->max()[1]);
	}
}

template <>
inline void viewer_of<clk::bounded<glm::vec3>>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f, %.3f", _data->val().x, _data->val().y, _data->val().z);
	if(extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", _data->min()[0], _data->max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", _data->min()[1], _data->max()[1]);
		ImGui::Text("Z (%.3f - %.3f)", _data->min()[2], _data->max()[2]);
	}
}

template <>
inline void viewer_of<clk::bounded<glm::vec4>>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", _data->val().x, _data->val().y, _data->val().z, _data->val().w);
	if(extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", _data->min()[0], _data->max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", _data->min()[1], _data->max()[1]);
		ImGui::Text("Z (%.3f - %.3f)", _data->min()[2], _data->max()[2]);
		ImGui::Text("W (%.3f - %.3f)", _data->min()[3], _data->max()[3]);
	}
}

template <>
inline void viewer_of<clk::color_rgb>::draw_contents() const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f", _data->r(), _data->g(), _data->b());
	if(extended_preferred())
	{
		auto s = available_width();

		ImGui::ColorButton("##", ImVec4(_data->r(), _data->g(), _data->b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", ImVec4(_data->r(), _data->g(), _data->b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void viewer_of<clk::color_rgba>::draw_contents() const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f, A:%.3f", _data->r(), _data->g(), _data->b(), _data->a());
	if(extended_preferred())
	{
		auto s = available_width();

		ImGui::ColorButton("##", *_data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", *_data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void viewer_of<std::chrono::nanoseconds>::draw_contents() const
{
	auto time_units = time_unit::decompose(*_data);
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
