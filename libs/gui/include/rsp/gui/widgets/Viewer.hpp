#pragma once

#include "rsp/gui/widgets/widget.hpp"
#include "rsp/util/bounded.hpp"
#include "rsp/util/color_rgb.hpp"
#include "rsp/util/color_rgba.hpp"
#include "rsp/util/time_unit.hpp"
#include "rsp/util/type_list.hpp"

#include <imgui.h>
#include <typeindex>
#include <unordered_map>

namespace rsp::gui
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
	static auto get_factories() -> std::unordered_map<std::uint64_t, factory>&;
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
	auto is_data_available() const noexcept -> bool final;
	void draw_contents() const override;

protected:
	auto get_data() const -> data_type const*;

private:
	data_type const* data;
};

template <typename data_type, typename viewer_implementation>
inline void viewer::register_factory()
{
	get_factories()[std::type_index(typeid(data_type)).hash_code()] =
		[](void const* data, std::string const& data_name) -> std::unique_ptr<viewer> {
		return std::make_unique<viewer_implementation>(static_cast<data_type const*>(data), data_name);
	};
}

inline auto viewer::create(std::size_t type_hash, void const* data, std::string const& data_name)
	-> std::unique_ptr<viewer>
{
	auto& factories = get_factories();
	if(factories.count(type_hash) == 0)
		return std::make_unique<viewer_of<void>>(data, data_name);
	return factories.at(type_hash)(data, data_name);
}

template <typename data_type>
auto viewer::create(data_type const* data, std::string const& data_name) -> std::unique_ptr<viewer>
{
	return viewer::create(std::type_index(typeid(data_type)).hash_code(), data, data_name);
}

inline auto viewer::get_factories() -> std::unordered_map<std::uint64_t, factory>&
{
	static auto factories = []() {
		std::unordered_map<std::uint64_t, factory> factories;

		using supported_types = meta::type_list<bool, int, float, glm::vec2, glm::vec3, glm::vec4, rsp::bounded<int>,
			rsp::bounded<float>, rsp::bounded<glm::vec2>, rsp::bounded<glm::vec3>, rsp::bounded<glm::vec4>,
			rsp::color_rgb, rsp::color_rgba, std::chrono::nanoseconds>;

		supported_types::for_each([&factories](auto* dummy) {
			using current_type = std::remove_cv_t<std::remove_pointer_t<decltype(dummy)>>;

			std::size_t hash = std::type_index(typeid(current_type)).hash_code();
			assert("Type hash function collision!" && factories.count(hash) == 0);

			factories[hash] = [](void const* data, std::string const& dataName) -> std::unique_ptr<viewer> {
				return std::make_unique<viewer_of<current_type>>(static_cast<current_type const*>(data), dataName);
			};
		});
		return factories;
	}();
	return factories;
}

inline viewer::viewer(std::string const& data_name) : widget(data_name)
{
}

template <typename data_type>
viewer_of<data_type>::viewer_of(data_type const* data, std::string const& data_name) : viewer(data_name), data(data)
{
}

template <typename data_type>
auto viewer_of<data_type>::clone() const -> std::unique_ptr<widget>
{
	auto cloned_editor = std::make_unique<viewer_of<data_type>>(data, get_data_name());
	return cloned_editor;
}

template <typename data_type>
void viewer_of<data_type>::update_data_pointer(void const* data)
{
	this->data = static_cast<data_type const*>(data);
}

template <typename data_type>
auto viewer_of<data_type>::is_data_available() const noexcept -> bool
{
	return data != nullptr;
}

template <typename data_type>
void viewer_of<data_type>::draw_contents() const
{
	ImGui::Text("NO VIEWER IMPLEMENTATION");
}

template <typename data_type>
auto viewer_of<data_type>::get_data() const -> data_type const*
{
	return data;
}

template <>
inline void viewer_of<bool>::draw_contents() const
{
	ImGui::SameLine();
	if(*data)
		ImGui::Text("On");
	else
		ImGui::Text("Off");
}

template <>
inline void viewer_of<int>::draw_contents() const
{
	ImGui::SameLine();
	ImGui::Text("%i", *data);
}

template <>
inline void viewer_of<float>::draw_contents() const
{
	ImGui::SameLine();
	ImGui::Text("%.3f", *data);
}

template <>
inline void viewer_of<glm::vec2>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f", data->x, data->y);
}

template <>
inline void viewer_of<glm::vec3>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f, %.3f", data->x, data->y, data->z);
}

template <>
inline void viewer_of<glm::vec4>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", data->x, data->y, data->z, data->w);
}

template <>
inline void viewer_of<rsp::bounded<int>>::draw_contents() const
{
	ImGui::Text("%i (%i - %i)", data->get_val(), data->get_min(), data->get_max());
}

template <>
inline void viewer_of<rsp::bounded<float>>::draw_contents() const
{
	ImGui::Text("%.3f (%.3f - %.3f)", data->get_val(), data->get_min(), data->get_max());
}

template <>
inline void viewer_of<rsp::bounded<glm::vec2>>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f", data->get_val().x, data->get_val().y);
	if(is_extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data->get_min()[0], data->get_max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data->get_min()[1], data->get_max()[1]);
	}
}

template <>
inline void viewer_of<rsp::bounded<glm::vec3>>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f, %.3f", data->get_val().x, data->get_val().y, data->get_val().z);
	if(is_extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data->get_min()[0], data->get_max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data->get_min()[1], data->get_max()[1]);
		ImGui::Text("Z (%.3f - %.3f)", data->get_min()[2], data->get_max()[2]);
	}
}

template <>
inline void viewer_of<rsp::bounded<glm::vec4>>::draw_contents() const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", data->get_val().x, data->get_val().y, data->get_val().z, data->get_val().w);
	if(is_extended_preferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data->get_min()[0], data->get_max()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data->get_min()[1], data->get_max()[1]);
		ImGui::Text("Z (%.3f - %.3f)", data->get_min()[2], data->get_max()[2]);
		ImGui::Text("W (%.3f - %.3f)", data->get_min()[3], data->get_max()[3]);
	}
}

template <>
inline void viewer_of<rsp::color_rgb>::draw_contents() const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f", data->r(), data->g(), data->b());
	if(is_extended_preferred())
	{
		auto s = get_available_width();

		ImGui::ColorButton("##", ImVec4(data->r(), data->g(), data->b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", ImVec4(data->r(), data->g(), data->b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void viewer_of<rsp::color_rgba>::draw_contents() const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f, A:%.3f", data->r(), data->g(), data->b(), data->a());
	if(is_extended_preferred())
	{
		auto s = get_available_width();

		ImGui::ColorButton("##", *data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", *data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void viewer_of<std::chrono::nanoseconds>::draw_contents() const
{
	auto time_units = time_unit::decompose(*data);
	if(is_extended_preferred())
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
		const int max_units_to_draw = 2;
		for(int i = 0; i < time_units.size(); i++)
		{
			if(time_units[i].value)
			{
				for(int j = i; j < time_units.size() && j - i < max_units_to_draw; j++)
				{
					ImGui::Text(("%i" + time_units[j].suffix).c_str(), time_units[j].value);
					ImGui::SameLine();
				}
				break;
			}
		}
		ImGui::NewLine();
	}
}

} // namespace rsp::gui
